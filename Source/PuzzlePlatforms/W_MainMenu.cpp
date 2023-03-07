// Copyright 2023 DME Games


#include "W_MainMenu.h"
#include "W_SessionInfo.h"
#include "GameFramework/PlayerController.h"
#include "CPPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"

UW_MainMenu::UW_MainMenu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SessionInfoWidget(TEXT("/Game/Widgets/WBP_SessionInfo"));
	if (SessionInfoWidget.Class != nullptr)
	{
		SessionInfoClass = SessionInfoWidget.Class;
	}	
}

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

void UW_MainMenu::UpdateSessionList(TArray<FSessionInfoStruct> SessionSearchResultsIn)
{
	if (SessionSearchResultsIn.Num() > 0)
	{
		const FString SessionNumText = "Number of sessions found: " + FString::FromInt(SessionSearchResultsIn.Num());
		tbSessionSearchInfo->SetText(FText::FromString(SessionNumText));

		uint32 i = 0;
		for (const FSessionInfoStruct& SessionData : SessionSearchResultsIn)
		/* TESTING PURPOSES ONLY
		for (int j = 0; j < 4; ++j)
		*/
		{
			UW_SessionInfo* WidgetToCreate = CreateWidget<UW_SessionInfo>(this, SessionInfoClass);
			const FText TextToAdd = FText::FromString(SessionData.SessionName);
			/* TESTING PURPOSES ONLY
			const FString StringToAdd = "Test" + FString::FromInt(j);
			const FText TextToAdd = FText::FromString(StringToAdd);
			END OF TESTING */
			WidgetToCreate->SetSessionInfoText(SessionData);
			WidgetToCreate->Setup(this, i);
			++i;
			sbSessionInfo->AddChild(WidgetToCreate);
		}
	}
	else
	{
		tbSessionSearchInfo->SetText(FText::FromString("No Valid Sessions Found"));
	}   
}

void UW_MainMenu::SelectSessionIndex(uint32 IndexIn)
{
	// Check that the value has been previously set, set it if not
	if (!SessionIndexSelected.IsSet())
	{
		SessionIndexSelected = IndexIn;	
	}
	else
	{
		// Clear the colour from the previously selected IndexIn, only if there is a valid sbSessionInfo and it is different
		if (sbSessionInfo && SessionIndexSelected.GetValue() != IndexIn)
		{
			TArray<UWidget*> SessionInfoChildren = sbSessionInfo->GetAllChildren();
			if (UW_SessionInfo* WidgetInstance = Cast<UW_SessionInfo>(SessionInfoChildren[SessionIndexSelected.GetValue()]))
			{
				WidgetInstance->SetColourToDefault();
			}

			// Set the new session index
			SessionIndexSelected = IndexIn;
			UE_LOG(LogTemp, Warning, TEXT("SelectedSessionIndex is %d"), *SessionIndexSelected);
		}

		
		
	}
		
	
	
}

bool UW_MainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (btnHOST)
	{
		btnHOST->OnClicked.AddDynamic(this, &UW_MainMenu::HostButtonClicked);
		btnHOST->SetIsEnabled(true);
	}
	else
	{
		Success = false;
	}

	if (btnJOIN)
	{
		btnJOIN->OnClicked.AddDynamic(this, &UW_MainMenu::JoinButtonClicked);
		btnJOIN->SetIsEnabled(true);
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

	if (tbHostNameEntry)
	{
		tbHostNameEntry->OnTextChanged.AddDynamic(this, &UW_MainMenu::OnHostTextChanged);
	}

	if (btnHostNameOK)
	{
		btnHostNameOK->OnClicked.AddDynamic(this, &UW_MainMenu::HostNameOKClicked);
	}

	if (btnHostNameCancel)
	{
		btnHostNameCancel->OnClicked.AddDynamic(this, &UW_MainMenu::HostNameCancelClicked);
	}
	
	if (btnJoinServerRefresh)
	{
		btnJoinServerRefresh->OnClicked.AddDynamic(this, &UW_MainMenu::UW_MainMenu::JoinButtonRefreshClicked);	
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
	if (WidgetSwitcher != nullptr)
	{
		WidgetSwitcher->SetActiveWidget(HostMenu);
	}
}

void UW_MainMenu::OnHostTextChanged(const FText& InText)
{
	if (btnHostNameOK && tbHostNameEntry)
	{
		if (InText.IsEmpty())
		{
			btnHostNameOK->SetIsEnabled(false);
		}
		else
		{
			
			if (!btnHostNameOK->GetIsEnabled())
			{
				btnHostNameOK->SetIsEnabled(true);
			}
		}
	}
}

void UW_MainMenu::HostNameOKClicked()
{
	if (GameInstanceRef != nullptr  && tbHostNameEntry)
	{
		if (!tbHostNameEntry->GetText().IsEmpty())
		{
			GameInstanceRef->Host(tbHostNameEntry->GetText().ToString());
		}
		else
		{
			tbHostNameEntry->SetFocus();
		}		
	}
}

void UW_MainMenu::HostNameCancelClicked()
{
	if (WidgetSwitcher != nullptr)
	{
		if (!tbHostNameEntry->GetText().IsEmpty())
		{
			tbHostNameEntry->SetText(FText::FromString(""));
		}

		WidgetSwitcher->SetActiveWidget(MainMenu);
	}
}

void UW_MainMenu::JoinButtonClicked()
{
	// Check that the widget switch is valid and move to it
	if (WidgetSwitcher != nullptr)
	{
		WidgetSwitcher->SetActiveWidget(LoginMenu);

		// Check that the Session Info scroll box and Session Info widget are valid
		if (sbSessionInfo && SessionInfoClass && GameInstanceRef)
		{
			// Check if there are any old sessions found and clear them
			if (sbSessionInfo->HasAnyChildren())
			{
				sbSessionInfo->ClearChildren();
			}

			// Update the session list
			JoinButtonRefreshClicked();
		}
	}
}

void UW_MainMenu::JoinButtonRefreshClicked()
{
	if (GameInstanceRef)
	{
		sbSessionInfo->ClearChildren();
		tbSessionSearchInfo->SetText(FText::FromString("Searching for sessions...."));
		GameInstanceRef->FindGameSessions();
	}
}

void UW_MainMenu::JoinButtonOKClicked()
{
	if (SessionIndexSelected.IsSet() && GameInstanceRef != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Index set to %d"), SessionIndexSelected.GetValue());
		GameInstanceRef->Join(SessionIndexSelected.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Sessions Index or Game Instance is not set in main menu"));
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
