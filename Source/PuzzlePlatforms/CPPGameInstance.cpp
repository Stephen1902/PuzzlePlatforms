// Copyright 2023 DME Games


#include "CPPGameInstance.h"
#include "Blueprint/UserWidget.h"

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

	if (UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuWidget))
	{
		Menu->AddToViewport();	
		Menu->bIsFocusable = true;

		APlayerController* PC = GetFirstLocalPlayerController();

		// Check there is a valid Player Controller
		if (!PC) { return; }

		PC->SetInputMode(FInputModeUIOnly());
		PC->SetShowMouseCursor(true);
		
	}
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
