// Copyright 2023 DME Games


#include "CPPGameInstance.h"
#include "W_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PauseMenu.h"
#include "OnlineSubsystem.h"

const static FName SESSION_NAME = TEXT("My Session"); 

UCPPGameInstance::UCPPGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidget(TEXT("/Game/Widgets/WBP_MainMenu"));
	if (MainMenuWidget.Class != nullptr)
	{
		MenuWidget = MainMenuWidget.Class;
	}

	// Get a reference to the pause menu
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBP(TEXT("/Game/Widgets/WBP_PauseMenu"));
	if (PauseMenuBP.Class != nullptr)
	{
		PauseMenuWidget = PauseMenuBP.Class;
	}
}

void UCPPGameInstance::Init()
{
	Super::Init();

	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("Online subsystem is %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCPPGameInstance::OnSessionCreated);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCPPGameInstance::OnSessionDestroyed);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCPPGameInstance::FindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCPPGameInstance::OnJoinSessionComplete);
			
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				FindGameSessions();				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Online susbystem is null"));
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

void UCPPGameInstance::OpenPauseMenu()
{
	// Check whether the pause menu has been created before
	if (PauseMenuRef == nullptr)
	{
		PauseMenuRef = CreateWidget<UPauseMenu>(this, PauseMenuWidget);
	}
	
	// Get the world and check it exists
	const UWorld* World = GetWorld();
	if (!World) { return; }

	// Get the current map name.  If not the main menu, open the pause menu 
	if (World->GetMapName() != "MainMenu")
	{
		PauseMenuRef->OpenPauseMenu(this);
	}
}

void UCPPGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		const FNamedOnlineSession* SessionNameInUse = SessionInterface->GetNamedSession(SESSION_NAME);
		if (SessionNameInUse != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}

		CreateSession();
		
	}
}

void UCPPGameInstance::Join(uint32 SessionRowIndex)
{
	APlayerController* PC = GetFirstLocalPlayerController();

	// Check there is a valid Player Controller and a valid session interface and valid session search
	if (!PC || !SessionInterface.IsValid() || !SessionSearch.IsValid()) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[SessionRowIndex]);
}

void UCPPGameInstance::LeaveJoin()
{
	APlayerController* PC = GetFirstLocalPlayerController();

	// Check there is a valid Player Controller
	if (!PC) { return; }

	const FString MainMenuLocation = "/Game/ThirdPerson/Maps/MainMenu.MainMenu";
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *MainMenuLocation));

	PC->ClientTravel(MainMenuLocation, TRAVEL_Absolute);
}

void UCPPGameInstance::QuitGame()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	const UWorld* World = GetWorld();

	if (!PC || !World) { return; }

	UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
}

TArray<FOnlineSessionSearchResult> UCPPGameInstance::GetSessionResults()
{
	return SessionResults;
}

void UCPPGameInstance::OnSessionCreated(FName SessionIn, bool bSessionCreatedSuccessfully)
{
	if (!bSessionCreatedSuccessfully)
	{
		return;
	}
		
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	// Check we got a valid world pointer
	if (!World) { return; }

	World->ServerTravel("/Game/ThirdPerson/Maps/DefaultMap?listen");
}

void UCPPGameInstance::OnSessionDestroyed(FName SessionIn, bool bSessionDestroyedSuccessfully)
{
	if (bSessionDestroyedSuccessfully)
	{
		CreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
	}
}

void UCPPGameInstance::FindSessionsComplete(bool bSearchCompleteSuccessfully)
{
	if (bSearchCompleteSuccessfully && MainMenuRef)
	{
		// Check whether any existing sessions are in the array, clear if they are
		if (SessionResults.Num() > 0) { SessionResults.Empty(); }

		// Add found results to the array
		SessionResults = SessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Sessions found: %i"), SessionResults.Num());
		// Populate the list in the main menu widget
		MainMenuRef->UpdateSessionList(SessionResults);

/*  For testing purposes only
		if (SessionResults.Num() > 0)
		{
			for (auto& It : SessionResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *It.GetSessionIdStr());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid sessions found"));
		}
*/
	}
}

void UCPPGameInstance::OnJoinSessionComplete(FName SessionIn, EOnJoinSessionCompleteResult::Type SessionCompleteResult)
{
	// Get the first (local) player controller
	APlayerController* PC = GetFirstLocalPlayerController();
	// Check there is a valid Player Controller
	if (!PC) { return; }
	// We need a string for Travel info.  This is an OUT parameter inside GetResolvedConnectString
	FString TravelURL;
	
	if (SessionInterface->GetResolvedConnectString(SessionIn, TravelURL))
	{
		PC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}

void UCPPGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UCPPGameInstance::FindGameSessions() const
{
	SessionSearch->bIsLanQuery = true;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
