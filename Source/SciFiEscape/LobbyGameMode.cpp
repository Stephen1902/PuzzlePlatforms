// Copyright 2023 DME Games


#include "LobbyGameMode.h"
#include "CPPGameInstance.h"
#include "SciFiEscapeCharacter.h"

void PrintString(const FString& StringToPrint)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, StringToPrint);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		// Travel to the game lobby, after a delay
		GetWorldTimerManager().SetTimer(TravelTimerHandle, this, &ALobbyGameMode::TravelToGameLevel, 10.f, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void ALobbyGameMode::TravelToGameLevel()
{
	if (UCPPGameInstance* GameInstance = Cast<UCPPGameInstance>(GetGameInstance()))
	{
		GameInstance->StartSession();
	}
	else
	{
		return;
	}
	
	UWorld* World = GetWorld();

	// Check we got a valid world pointer
	if (!World) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/DefaultMap?Listen");
}
