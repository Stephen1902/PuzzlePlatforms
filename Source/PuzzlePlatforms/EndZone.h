// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndZone.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AEndZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndZone();
	
	UPROPERTY(EditAnywhere, Category="Set Up")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category="Set Up")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category="Set Up")
	UStaticMeshComponent* DownComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	int32 NumberOfPlayersInteracting;
	int32 NumberOfPlayersNeededToWin;

	TArray<class APuzzlePlatformsCharacter*> PlayersInteracting; 

};
