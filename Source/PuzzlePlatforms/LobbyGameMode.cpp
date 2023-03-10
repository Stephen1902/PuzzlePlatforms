// Copyright 2023 DME Games


#include "LobbyGameMode.h"

#include "CPPGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;
	
	if (NumberOfPlayers >= 2)
	{
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
