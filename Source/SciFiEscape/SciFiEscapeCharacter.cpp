// Copyright Epic Games, Inc. All Rights Reserved.

#include "SciFiEscapeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CPPGameInstance.h"
#include "InteacteractiveActorBase.h"
#include "W_PlayerWidget.h"

//////////////////////////////////////////////////////////////////////////
// ASciFiEscapeCharacter

ASciFiEscapeCharacter::ASciFiEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a first person style camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh());
	FirstPersonCamera->bUsePawnControlRotation = true;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	}

void ASciFiEscapeCharacter::HasWonGame() const
{
	GetMovementComponent()->StopMovementImmediately();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->SetIgnoreMoveInput(true);
	}
}

void ASciFiEscapeCharacter::DestroyChar()
{
	Destroy();
}

void ASciFiEscapeCharacter::SetGameIsRunning()
{
	bGameIsRunning = true;
	FollowCamera->Deactivate();
	FirstPersonCamera->SetActive(true);
	GetMesh()->SetOwnerNoSee(true);
	
	if (PlayerWidget)
	{
		PlayerWidgetRef = CreateWidget<UW_PlayerWidget>(GetWorld(), PlayerWidget);
		PlayerWidgetRef->AddToViewport();
	}
}


void ASciFiEscapeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Called on the server to show to all players when a mesh changes 
	if (HasAuthority())
	{
		GetMesh()->SetIsReplicated(true);
	}

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASciFiEscapeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameIsRunning)
	{
		TimeSinceLastInteraction += DeltaSeconds;
	
		if (TimeSinceLastInteraction > TimeBetweenInteractionCheck)
		{
			CheckForInteractive();
		}
	}
}

void ASciFiEscapeCharacter::OpenPauseMenu()
{
	if (!GameInstanceRef) { GetGameInstance(); }
	// Check we successfully got the main menu class from the code in the initializer
	if (!ensure(GameInstanceRef != nullptr)) return;

	bGameIsRunning = false;
	GameInstanceRef->OpenPauseMenu();
}

void ASciFiEscapeCharacter::GetGameInstance()
{
	// Get the world and check it is valid
	const UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	// Populate the game instance reference
	GameInstanceRef = Cast<UCPPGameInstance>(World->GetGameInstance());
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASciFiEscapeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASciFiEscapeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASciFiEscapeCharacter::Look);

		// Open Pause Menu
		EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &ASciFiEscapeCharacter::OpenPauseMenu);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASciFiEscapeCharacter::TryToInteract);
	}
}

void ASciFiEscapeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASciFiEscapeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASciFiEscapeCharacter::CheckForInteractive()
{
	TimeSinceLastInteraction = 0.f;

	FVector EyesLoc;
	FRotator EyesRot;
	GetActorEyesViewPoint(EyesLoc, EyesRot);
	float InteractiveCheckLength = 200.f;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	const FVector StartLoc = EyesLoc;
	const FVector EndLoc = StartLoc + (EyesRot.Vector() * InteractiveCheckLength);
	
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_GameTraceChannel1, QueryParams))
	{
		AInteractiveActorBase* InteractiveHit = Cast<AInteractiveActorBase>(HitResult.GetActor()); 
		if (InteractiveHit != CurrentInteractive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit something interactive"));
			CurrentInteractive = InteractiveHit;
			if (PlayerWidgetRef)
			{
				PlayerWidgetRef->InteractiveItemFound();
			}
		}
	}
	else
	{
		CurrentInteractive = nullptr;
		if (PlayerWidgetRef)
		{
			PlayerWidgetRef->InteractiveItemLost();
		}
	}
	
}

void ASciFiEscapeCharacter::TryToInteract()
{
	if (CurrentInteractive != nullptr)
	{
		CurrentInteractive->Interact();
	}
}



