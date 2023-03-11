// Copyright 2023 DME Games

#include "CPPGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PauseMenu.h"
#include "OnlineSubsystem.h"

const static FName SERVER_SESSION_NAME = TEXT("ServerName");
const static FString MAIN_MENU = "/Game/Maps/MainMenu.MainMenu";

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

	if (GEngine)
	{
		GetEngine()->OnNetworkFailure().AddUObject(this, &UCPPGameInstance::OnNetworkFailure);
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

void UCPPGameInstance::Host(FString HostNameIn)
{
	if (SessionInterface.IsValid())
	{
		const FNamedOnlineSession* SessionNameInUse = SessionInterface->GetNamedSession(NAME_GameSession);
		if (SessionNameInUse != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}

		EnteredHostName = HostNameIn;
		CreateSession();
		
	}
}

void UCPPGameInstance::Join(uint32 SessionRowIndex)
{
	APlayerController* PC = GetFirstLocalPlayerController();

	// Check there is a valid Player Controller and a valid session interface and valid session search
	if (!PC || !SessionInterface.IsValid() || !SessionSearch.IsValid()) { return; }

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionRowIndex]);
}

void UCPPGameInstance::LeaveJoin()
{
	APlayerController* PC = GetFirstLocalPlayerController();

	// Check there is a valid Player Controller
	if (!PC) { return; }
	
	PC->ClientTravel(MAIN_MENU, TRAVEL_Absolute);
}

void UCPPGameInstance::QuitGame()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	const UWorld* World = GetWorld();

	if (!PC || !World) { return; }

	UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, false);
}

void UCPPGameInstance::OnSessionCreated(FName SessionIn, bool bSessionCreatedSuccessfully)
{
	if (!bSessionCreatedSuccessfully)
	{
		return;
	}
	
	UWorld* World = GetWorld();

	// Check we got a valid world pointer
	if (!World) { return; }

	World->ServerTravel("/Game/Maps/Lobby?Listen");
}

void UCPPGameInstance::OnSessionDestroyed(FName SessionIn, bool bSessionDestroyedSuccessfully)
{
	if (bSessionDestroyedSuccessfully && EnteredHostName.Len() > 0)
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
		TArray<FSessionInfoStruct> SessionInfoArray;
		for (const FOnlineSessionSearchResult& SessionInfo : SessionSearch->SearchResults)
		{
			// Add the session ID to SessionResults array
			SessionResults.Add(SessionInfo.GetSessionIdStr());

			// Populate the SessionInfo Struct with information
			FSessionInfoStruct ThisSessionInfo;
			
			ThisSessionInfo.MaxPlayers = SessionInfo.Session.SessionSettings.NumPublicConnections;
			ThisSessionInfo.CurrentPlayers = ThisSessionInfo.MaxPlayers - SessionInfo.Session.NumOpenPublicConnections;
			ThisSessionInfo.HostUserName = SessionInfo.Session.OwningUserName;
			FString FoundServerName; 
			if (SessionInfo.Session.SessionSettings.Get(SERVER_SESSION_NAME, FoundServerName))
			{
				ThisSessionInfo.SessionName = FoundServerName;
			}
			else
			{
				ThisSessionInfo.SessionName = SessionInfo.Session.GetSessionIdStr();
			}
			SessionInfoArray.Add(ThisSessionInfo);			
		}
		// Populate the list in the main menu widget
		MainMenuRef->UpdateSessionList(SessionInfoArray);
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

void UCPPGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	LeaveJoin();
}

void UCPPGameInstance::CreateSession() const
{
	if (SessionInterface.IsValid() && EnteredHostName.Len() > 0)
	{
		FOnlineSessionSettings SessionSettings;
		
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 3;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_SESSION_NAME, EnteredHostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);

		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, *EnteredHostName);
	}
}


void UCPPGameInstance::FindGameSessions() const
{
	if (SessionInterface.IsValid())
	{
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSearch->bIsLanQuery = true;
			UE_LOG(LogTemp, Warning, TEXT("LAN Query is true"))
		}
		else
		{
			SessionSearch->bIsLanQuery = false;
		}
		
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCPPGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(NAME_GameSession);
	}
}
