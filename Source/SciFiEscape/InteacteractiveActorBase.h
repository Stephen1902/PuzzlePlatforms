// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteacteractiveActorBase.generated.h"

UCLASS()
class SCIFIESCAPE_API AInteractiveActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	bool bSingleUseOnly;
	
	// Blueprint function for when this actor is interacted with
	UFUNCTION(BlueprintImplementableEvent)
	void InteractBP();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player tries to interact with this actor
	void Interact();

private:
	bool bHasExpired;

};
