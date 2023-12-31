// Copyright 2023 DME Games


#include "W_PauseMenu.h"
#include "CPPGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UW_PauseMenu::OpenPauseMenu(UCPPGameInstance* GameInstanceIn)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("OpenW_PauseMenu called"));

	GameInstanceRef = GameInstanceIn;
	// Check a valid player was given when called
	if (GameInstanceRef)
	{
		this->AddToViewport();	
		this->SetIsFocusable(true);

		// Get the player controller
		APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

		// Check there is a valid Player Controller
		if (!PlayerController) { return; }

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
}

void UW_PauseMenu::ClosePauseMenu()
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

bool UW_PauseMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (btnQUIT)
	{
		btnQUIT->OnClicked.AddDynamic(this, &UW_PauseMenu::QuitButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnBACK)
	{
		btnBACK->OnClicked.AddDynamic(this, &UW_PauseMenu::BackButtonClicked);
	}
	else
	{
		Success = false;
	}
	
	if (btnQuitDesktop)
	{
		btnQuitDesktop->OnClicked.AddDynamic(this, &UW_PauseMenu::QuitDesktopClicked);		
	}
	else
	{
		Success = false;
	}

	if (btnQuitMainMenu)
	{
		btnQuitMainMenu->OnClicked.AddDynamic(this, &UW_PauseMenu::QuitMainMenuClicked);		
	}
	else
	{
		Success = false;
	}

	if (btnQuitCancel)
	{
		btnQuitCancel->OnClicked.AddDynamic(this, &UW_PauseMenu::QuitCancelClicked);		
	}
	else
	{
		Success = false;
	}

	return Success;
}

void UW_PauseMenu::QuitButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(QuitConfirmMenu);
}

void UW_PauseMenu::BackButtonClicked()
{
	ClosePauseMenu();
}

void UW_PauseMenu::QuitDesktopClicked()
{
	if (ensure(GameInstanceRef))
	{
		GameInstanceRef->QuitGame();
	}
}

void UW_PauseMenu::QuitMainMenuClicked()
{
	if (GameInstanceRef)
	{
		GameInstanceRef->LeaveJoin();
	}
}

void UW_PauseMenu::QuitCancelClicked()
{
	WidgetSwitcher->SetActiveWidget(PauseMenu);
}
