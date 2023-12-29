// Copyright Epic Games, Inc. All Rights Reserved.

#include "SciFiEscapeGameMode.h"
#include "SciFiEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASciFiEscapeGameMode::ASciFiEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_Char1"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}