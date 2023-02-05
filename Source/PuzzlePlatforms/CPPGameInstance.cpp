// Copyright 2023 DME Games


#include "CPPGameInstance.h"

UCPPGameInstance::UCPPGameInstance(const FObjectInitializer& ObjectInitializer)
{
	
}

void UCPPGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	// Check we got a valid world pointer
	if (!World) { return; }

	World->ServerTravel("/Game/ThirdPerson/Maps/DefaultMap?listen");
}

void UCPPGameInstance::Join(const FString& AddressIn)
{
	APlayerController* PC = GetFirstLocalPlayerController();

	// Check there is a valid Player Controller
	if (!PC) { return; }

	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *AddressIn));
	PC->ClientTravel(AddressIn, TRAVEL_Absolute);
}
