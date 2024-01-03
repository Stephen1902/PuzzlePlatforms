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
	FText InteractiveInstructionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	bool bSingleUseOnly;
	
	// Blueprint function for when this actor is interacted with
	UFUNCTION(BlueprintImplementableEvent)
	void InteractBP();

	// Called by the Blueprint when the required action is complete
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void BPSuccessful();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player tries to interact with this actor
	void Interact();

	// Called to populate the TextBoxInteractInstructions Text Box on the Player Widget
	FText GetInteractiveInstructionText() const { return InteractiveInstructionText; }

private:
	bool bHasExpired;

};
