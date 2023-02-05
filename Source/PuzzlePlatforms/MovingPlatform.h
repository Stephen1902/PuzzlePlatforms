// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

	AMovingPlatform();
	
public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();
	void SetPlatformColour(FLinearColor ColourIn);
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	// Amount of cm to move per second in game
	UPROPERTY(EditAnywhere, Category = "Set Up")
	double AmountToMovePerSecond;

	// Location this object will move to in the world
	UPROPERTY(EditAnywhere, Category = "Set Up", meta=(MakeEditWidget=true))
	FVector LocationToMoveTo;

	// Dictates whether this item returns to it's starting position when not triggered ie for a door that automatically closes
	UPROPERTY(EditAnywhere, Category = "Set Up")
	bool bAutoReturns;
	
	UPROPERTY(EditAnywhere, Category = "Set up")
	int32 TriggersNeededToActivate = 0;

	// If a trigger is needed to activate, set whether the colour matches that of the trigger frame
	UPROPERTY(EditAnywhere, Category = "Set up", meta=(EditCondition="TriggersrNeededToActivate>0"))
	bool bMatchTriggerColour;
private:
	void MoveMesh(float DeltaSeconds);

	// World location of actor at start of play
	UPROPERTY()
	FVector StartingPoint;

	// World location of target location
	UPROPERTY()
	FVector EndPoint;

	// Material if activated by a trigger
	UPROPERTY()
	UMaterial* PlatformMaterial;

	UPROPERTY()
	FLinearColor PlatformColour;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMat;

	bool bCanMove;
	bool bReturnToOrigin;
	int32 ActiveTriggers = 0;
};
