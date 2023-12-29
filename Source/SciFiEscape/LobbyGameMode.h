// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "SciFiEscapeGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SCIFIESCAPE_API ALobbyGameMode : public ASciFiEscapeGameMode
{
	GENERATED_BODY()

protected:
	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	virtual void Logout(AController* Exiting) override;

private:
	void TravelToGameLevel();
	
	uint32 NumberOfPlayers = 0;

	FTimerHandle TravelTimerHandle;
};
