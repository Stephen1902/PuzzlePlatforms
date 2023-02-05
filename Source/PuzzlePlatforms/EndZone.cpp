// Copyright 2023 DME Games


#include "EndZone.h"
#include "PuzzlePlatformsCharacter.h"
#include "Components/BoxComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GeometryCollection/GeometryCollectionSimulationCoreTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndZone::AEndZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(128.f, 128.f, 256.f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(RootComponent);
	
	NumberOfPlayersInteracting = 0;
	NumberOfPlayersNeededToWin = 2;
}

// Called when the game starts or when spawned
void AEndZone::BeginPlay()
{
	Super::BeginPlay();

	// Add functions to be called when the capsule is overlapped by a player
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndZone::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEndZone::OnEndOverlap);
}

void AEndZone::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APuzzlePlatformsCharacter* PlayerCharacter = Cast<APuzzlePlatformsCharacter>(OtherActor))
	{
		PlayersInteracting.Add(PlayerCharacter);
		NumberOfPlayersInteracting += 1;

		if (NumberOfPlayersInteracting >= NumberOfPlayersNeededToWin)
		{
			for (int32 i = 0; i < PlayersInteracting.Num(); ++i)
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), i))
				{
					PC->SetInputMode(FInputModeUIOnly());
				}
				if (PlayersInteracting[i]->HasAuthority())
				{
					// This is the server.  Call the end game function.
					GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, TEXT("YOU WIN!"));
				}
			}
		}
	}
}

void AEndZone::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APuzzlePlatformsCharacter* PlayerCharacter = Cast<APuzzlePlatformsCharacter>(OtherActor))
	{
		PlayersInteracting.Remove(PlayerCharacter);
		NumberOfPlayersInteracting = FMath::Clamp(NumberOfPlayersInteracting, 0, NumberOfPlayersInteracting - 1);
	}
}
