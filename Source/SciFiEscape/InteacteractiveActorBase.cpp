// Copyright 2023 DME Games

#include "InteacteractiveActorBase.h"

// Sets default values
AInteractiveActorBase::AInteractiveActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Block);

	bSingleUseOnly = true;
	InteractiveInstructionText = FText::FromString("Interactive Item");
}

// Called when the game starts or when spawned
void AInteractiveActorBase::BeginPlay()
{
	Super::BeginPlay();

	// All players need to see what this actor does in the world.
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AInteractiveActorBase::BPSuccessful()
{
	
}

// Called every frame
void AInteractiveActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActorBase::Interact()
{
	if (bSingleUseOnly)
	{
		MeshComp->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Ignore);
	}
	
	InteractBP();
}

