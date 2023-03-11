// Copyright 2023 DME Games


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;
	
	if (NumberOfPlayers >= 3)
	{
		UWorld* World = GetWorld();

		// Check we got a valid world pointer
		if (!World) { return; }

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/DefaultMap?Listen");
	}
	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}
