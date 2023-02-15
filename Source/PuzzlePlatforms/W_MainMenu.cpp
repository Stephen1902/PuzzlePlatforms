// Copyright 2023 DME Games


#include "W_MainMenu.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "CPPGameInstance.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UW_MainMenu::SetGameInstance(UCPPGameInstance* GameInstanceIn)
{
	GameInstanceRef = GameInstanceIn;
}

void UW_MainMenu::SetupMenu()
{
	if (GameInstanceRef)
	{
		this->AddToViewport();	
		this->bIsFocusable = true;

		APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

		// Check there is a valid Player Controller
		if (!PlayerController) { return; }

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game instance is not valid on Main Menu"));
	}

}

bool UW_MainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (btnHOST)
	{
		btnHOST->OnClicked.AddDynamic(this, &UW_MainMenu::HostButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnJOIN)
	{
		btnJOIN->OnClicked.AddDynamic(this, &UW_MainMenu::JoinButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnQUIT)
	{
		btnQUIT->OnClicked.AddDynamic(this, &UW_MainMenu::QuitButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnJoinServerOK)
	{
		btnJoinServerOK->OnClicked.AddDynamic(this, &UW_MainMenu::JoinButtonOKClicked);
	}
	else
	{
		Success = false;
	}

	if (btnJoinServerCancel)
	{
		btnJoinServerCancel->OnClicked.AddDynamic(this, &UW_MainMenu::JoinButtonCancelClicked);
	}
	else
	{
		Success = false;
	}

	if (btnQuitGameYes)
	{
		btnQuitGameYes->OnClicked.AddDynamic(this, &UW_MainMenu::UW_MainMenu::QuitGameYesClicked);
	}
	else
	{
		Success = false;
	}

	if (btnQuitGameNo)
	{
		btnQuitGameNo->OnClicked.AddDynamic(this, &UW_MainMenu::UW_MainMenu::QuitGameNoClicked);
	}
	else
	{
		Success = false;
	}
	
	
	return Success;
}

void UW_MainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PlayerController = GameInstanceRef->GetFirstLocalPlayerController();

	// Check there is a valid Player Controller
	if (!PlayerController) { return; }

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);

	this->RemoveFromParent();	
}

void UW_MainMenu::HostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Button Clicked"));

	if (GameInstanceRef != nullptr)
	{
		GameInstanceRef->Host();
	}
}

void UW_MainMenu::JoinButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Button Clicked"));

	if (WidgetSwitcher != nullptr)
	{
		WidgetSwitcher->SetActiveWidget(LoginMenu);
	}
}

void UW_MainMenu::JoinButtonOKClicked()
{
	if (tbAddressEntry && GameInstanceRef)
	{
		GameInstanceRef->Join(tbAddressEntry->GetText().ToString());
	}
}

void UW_MainMenu::JoinButtonCancelClicked()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(MainMenu);
	}
}

void UW_MainMenu::QuitGameYesClicked()
{
	if (GameInstanceRef != nullptr)
	{
		GameInstanceRef->QuitGame();
	}
}

void UW_MainMenu::QuitGameNoClicked()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(MainMenu);
	}
}

void UW_MainMenu::QuitButtonClicked()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidget(QuitMenu);
	}
}
