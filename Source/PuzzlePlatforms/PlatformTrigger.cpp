// Copyright 2023 DME Games

#include "PlatformTrigger.h"
#include "MovingPlatform.h"
#include "PuzzlePlatformsCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(128.f, 128.f, 256.f));

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame Mesh"));
	FrameMesh->SetupAttachment(RootComponent);
	FrameMesh->SetGenerateOverlapEvents(false);
	
	MovingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moving Mesh"));
	MovingMesh->SetupAttachment(FrameMesh);
	MovingMesh->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMat(TEXT("'/Game/Materials/M_TriggerFrame.M_TriggerFrame'"));
	if (FoundMat.Succeeded())
	{
		FrameMaterial = FoundMat.Object;
	}
	
	// Set the default colour to white
	FrameColour = FLinearColor(1.f, 1.f, 1.f, 1.f);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	// Add functions to be called when the capsule is overlapped by a player
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);

	// Called on the server to show to all players the trigger moving 
	if (HasAuthority())
	{
		SetReplicates(true);
		// Must have replication enabled on the moving parts or it won't appear on clients
		MovingMesh->SetIsReplicated(true);
	}

	NumberOfOverlappingPlayers = 0;
	StartPoint = MovingMesh->GetRelativeLocation();
	EndPoint = MovingMesh->GetRelativeLocation();
	EndPoint.Z -= 25.f;
	bCanMove = false;

	UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(FrameMaterial, this);
	DynamicMat->SetVectorParameterValue("FrameColour", FrameColour);
	FrameMesh->SetMaterial(0, DynamicMat);

	// If there is a platform being used by this trigger, try to set the colour to match
	if (PlatformsToMove.Num() > 0)
	{
		SetPlatformColour();
	}
}

void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check there are platforms to move
	if (PlatformsToMove.Num() > 0)
	{
		// Check what overlapped was a player 
		if (APuzzlePlatformsCharacter* CharacterThatHit = Cast<APuzzlePlatformsCharacter>(OtherActor))
		{
			for (int32 i = 0; i < PlatformsToMove.Num(); ++i)
			{
				PlatformsToMove[i]->AddActiveTrigger();
			}

			// Add an overlapping player
			NumberOfOverlappingPlayers += 1;
			// Only move if there isn't already a character standing on the trigger
			if (NumberOfOverlappingPlayers == 1)
			{
				bCanMove = true;
			}
		}
	}
}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check there are platforms to move
	if (PlatformsToMove.Num() > 0)
	{
		// Check what overlapped was a player 
		if (APuzzlePlatformsCharacter* CharacterThatHit = Cast<APuzzlePlatformsCharacter>(OtherActor))
		{
			for (int32 i = 0; i < PlatformsToMove.Num(); ++i)
			{
				PlatformsToMove[i]->RemoveActiveTrigger();
			}

			// Remove an overlapping player, clamping the value so it can never be less than 0 
			NumberOfOverlappingPlayers = FMath::Clamp(NumberOfOverlappingPlayers, 0, NumberOfOverlappingPlayers -= 1);
			// Only move if there are no characters standing on the trigger
			if (NumberOfOverlappingPlayers == 0)
			{
				bCanMove = true;
			}
		}
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run only on the server
	if (HasAuthority() && bCanMove)
	{
		MoveMesh(DeltaTime);
	}
}

void APlatformTrigger::SetPlatformColour()
{
	for (int32 i = 0; i < PlatformsToMove.Num(); ++i)
	{
		PlatformsToMove[i]->SetPlatformColour(FrameColour);
	}

}

void APlatformTrigger::MoveMesh(float DeltaSeconds)
{
	FVector CurrentLocation = MovingMesh->GetRelativeLocation();
	FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
	FVector PointToStopAt = EndPoint;

	// If there are no players on the platform, return it to it's origin point 
	if (NumberOfOverlappingPlayers == 0)
	{
		Direction = (StartPoint - EndPoint).GetSafeNormal();
		PointToStopAt = StartPoint;
	}
			
	CurrentLocation += (100 * DeltaSeconds * Direction);
	MovingMesh->SetRelativeLocation(CurrentLocation);
		
	// Check if the current location and end point are almost equal, swap around if they are
	if (FVector::PointsAreNear(CurrentLocation, PointToStopAt, 2.f))
	{
		bCanMove = false;
	}
}
