// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	// Tells this actor that it can tick.  Can be turned off to improve performance
	PrimaryActorTick.bCanEverTick = true;

	this->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMat(TEXT("'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'"));
	if (FoundMat.Succeeded())
	{
		PlatformMaterial = FoundMat.Object;
	}
	
	AmountToMovePerSecond = 25.0f;
	LocationToMoveTo = FVector(0.f);
	bAutoReturns = false;
	bCanMove = false;
	bReturnToOrigin = false;
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers += 1;

	// Do not allow an auto returning item ie a door, to move again
	if (ActiveTriggers > 1 && bAutoReturns) { return; }
	
	bReturnToOrigin = false;

	if (ActiveTriggers >= TriggersNeededToActivate)
	{
		bCanMove = true;
	}		
}

void AMovingPlatform::RemoveActiveTrigger()
{
	// Check to ensure the active triggers cannot go below zero
	ActiveTriggers = FMath::Clamp(ActiveTriggers, 0, ActiveTriggers - 1);

	// Check if there are any triggers active for this item
	if (ActiveTriggers < TriggersNeededToActivate)
	{
		// Check if the item should return to it's start position ie an auto closing door
		if (bAutoReturns)
		{
			// If an item that returns, return it
			bCanMove = true;
			bReturnToOrigin = true;
		}
		else
		{
			// Else stop the item from moving
			bCanMove = false;
		}
	}

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	StartingPoint = GetActorLocation();
	EndPoint = GetTransform().TransformPosition(LocationToMoveTo);
	if (TriggersNeededToActivate == 0)
	{
		bCanMove = true;
	}
	
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveMesh(DeltaSeconds);
}

void AMovingPlatform::SetPlatformColour(FLinearColor ColourIn)
{
	// Only create the dynamic material if one does not already exist
	if (bMatchTriggerColour && DynamicMat == nullptr)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(PlatformMaterial, this);
		DynamicMat->SetVectorParameterValue("PlatformColour", ColourIn);
		GetStaticMeshComponent()->SetMaterial(0, DynamicMat);
	}
}

void AMovingPlatform::MoveMesh(float DeltaSeconds)
{
	// Run only on the server
	if (HasAuthority() && bCanMove)
	{
		if (ActiveTriggers >= TriggersNeededToActivate || bAutoReturns)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector Direction = (EndPoint - StartingPoint).GetSafeNormal();
			FVector PointToStopAt = EndPoint;

			// If the item is set to auto return, change the variables that control this
			if (bReturnToOrigin)
			{
				Direction = (StartingPoint - EndPoint).GetSafeNormal();
				PointToStopAt = StartingPoint;
			}
			
			CurrentLocation += (AmountToMovePerSecond * DeltaSeconds * Direction);
			SetActorLocation(CurrentLocation);
			
			// Check if the current location and end point are almost equal, swap around if they are
			if (FVector::PointsAreNear(CurrentLocation, PointToStopAt, 2.f))
			{
				if (!bAutoReturns)
				{
					Swap(StartingPoint, EndPoint);
				}
				else
				{
					bCanMove = false;
				}
			}
		}
	}
}
