// Copyright 2023 DME Games


#include "CPPGameInstance.h"
#include "W_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

UCPPGameInstance::UCPPGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidget(TEXT("/Game/Widgets/WBP_MainMenu"));
	if (MainMenuWidget.Class != nullptr)
	{
		MenuWidget = MainMenuWidget.Class;
	}
}

void UCPPGameInstance::DisplayMainMenu()
{
	// Check we successfully got the main menu class from the code in the initializer
	if (!ensure(MenuWidget != nullptr)) return; 

	MainMenuRef = CreateWidget<UW_MainMenu>(this, MenuWidget);
	if (MainMenuRef != nullptr)
	{
		// Add the game instance reference to the created menu
		MainMenuRef->SetGameInstance(this);

		// Set up the menu in the menu class
		MainMenuRef->SetupMenu();
	}
}

void UCPPGameInstance::Host()
{
	if (MainMenuRef != nullptr)
	{
		
	}
	
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

void UCPPGameInstance::QuitGame()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	const UWorld* World = GetWorld();

	if (!PC || !World) { return; }

	UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
}
