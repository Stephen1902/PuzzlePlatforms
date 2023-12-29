// Copyright 2023 DME Games


#include "InteacteractiveActorBase.h"

// Sets default values
AInteacteractiveActorBase::AInteacteractiveActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteacteractiveActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteacteractiveActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

