// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UCPPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCPPGameInstance(const FObjectInitializer & ObjectInitializer);

	// Function to show the main menu
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void DisplayMainMenu();
	
	// Function for a host server
	UFUNCTION(Exec)
	void Host();

	// Function to join a host
	UFUNCTION(Exec)
	void Join(const FString& AddressIn);

private:
	TSubclassOf<class UUserWidget> MenuWidget;
};
