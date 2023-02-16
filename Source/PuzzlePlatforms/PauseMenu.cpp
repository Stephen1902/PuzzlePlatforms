// Copyright 2023 DME Games


#include "PauseMenu.h"
#include "CPPGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UPauseMenu::OpenPauseMenu(UCPPGameInstance* GameInstanceIn)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("OpenPauseMenu called"));

	GameInstanceRef = GameInstanceIn;
	// Check a valid player was given when called
	if (GameInstanceRef)
	{
		this->AddToViewport();	
		this->bIsFocusable = true;

		// Get the player controller
		APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

		// Check there is a valid Player Controller
		if (!PlayerController) { return; }

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
}

void UPauseMenu::ClosePauseMenu()
{
	// Check a valid player was given when called
	if (GameInstanceRef)
	{
		// Get the player controller
		APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

		// Check there is a valid Player Controller
		if (!PlayerController) { return; }

		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);

		this->RemoveFromParent();	
	}
}

bool UPauseMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (btnQUIT)
	{
		btnQUIT->OnClicked.AddDynamic(this, &UPauseMenu::QuitButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnBACK)
	{
		btnBACK->OnClicked.AddDynamic(this, &UPauseMenu::BackButtonClicked);
	}
	else
	{
		Success = false;
	}
	
	if (btnQuitDesktop)
	{
		btnQuitDesktop->OnClicked.AddDynamic(this, &UPauseMenu::QuitDesktopClicked);		
	}
	else
	{
		Success = false;
	}

	if (btnQuitMainMenu)
	{
		btnQuitMainMenu->OnClicked.AddDynamic(this, &UPauseMenu::QuitMainMenuClicked);		
	}
	else
	{
		Success = false;
	}

	if (btnQuitCancel)
	{
		btnQuitCancel->OnClicked.AddDynamic(this, &UPauseMenu::QuitCancelClicked);		
	}
	else
	{
		Success = false;
	}

	return Success;
}

void UPauseMenu::QuitButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(QuitConfirmMenu);
}

void UPauseMenu::BackButtonClicked()
{
	ClosePauseMenu();
}

void UPauseMenu::QuitDesktopClicked()
{
	if (ensure(GameInstanceRef))
	{
		GameInstanceRef->QuitGame();
	}
}

void UPauseMenu::QuitMainMenuClicked()
{
	if (GameInstanceRef)
	{
		GameInstanceRef->LeaveJoin();
	}
}

void UPauseMenu::QuitCancelClicked()
{
	WidgetSwitcher->SetActiveWidget(PauseMenu);
}
