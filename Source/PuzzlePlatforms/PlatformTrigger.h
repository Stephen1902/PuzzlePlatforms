// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformTrigger();

	UPROPERTY(EditAnywhere, Category = "Set Up")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Set Up")
	UStaticMeshComponent* FrameMesh;
	
	UPROPERTY(EditAnywhere, Category = "Set Up")
	UStaticMeshComponent* MovingMesh;

	// Colour of this frame in the world
	UPROPERTY(EditAnywhere, Category = "Set Up")
	FLinearColor FrameColour;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatform*> PlatformsToMove;

	// Starting Location for this item
	UPROPERTY()
	FVector StartPoint;

	// End Location for this item when moving
	UPROPERTY()
	FVector EndPoint;

	// Material to be set on the items by default
	UPROPERTY()
	UMaterial* FrameMaterial;

	// If there is a controlled platform, set the colour
	void SetPlatformColour();

	// Used to decide whether to drop the actor into the floor as if it had been stood on.
	int32 NumberOfOverlappingPlayers;

	void MoveMesh(float DeltaSeconds);

	bool bCanMove;
};
