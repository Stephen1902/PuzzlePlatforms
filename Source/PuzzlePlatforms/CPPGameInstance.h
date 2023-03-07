// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "W_MainMenu.h"
#include "CPPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UCPPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCPPGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	// Function to show the main menu
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void DisplayMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void OpenPauseMenu();
	
	// Function for a host server
	UFUNCTION(Exec)
	void Host(FString HostNameIn);

	// Function to join a host
	UFUNCTION(Exec)
	void Join(uint32 SessionRowIndex);
	
	UFUNCTION(Exec)
	void LeaveJoin();
	
	// Function to quit the game
	UFUNCTION(Exec)
	void QuitGame();

	//TArray<FSessionInfoStruct> GetSessionResults();

	void FindGameSessions() const;

private:
	TSubclassOf<UUserWidget> MenuWidget;

	UPROPERTY()
	TObjectPtr<class UW_MainMenu> MainMenuRef;

	TSubclassOf<UUserWidget> PauseMenuWidget;
	
	UPROPERTY()
	TObjectPtr<class UPauseMenu> PauseMenuRef;

	// Function to call when an online session has successfully be created
	void OnSessionCreated(FName SessionIn, bool bSessionCreatedSuccessfully);

	// Function to call when an online session has successfully been destroyed
	void OnSessionDestroyed(FName SessionIn, bool bSessionDestroyedSuccessfully);

	// Function to call when the game has finished searching for sessions to join
	void FindSessionsComplete(bool bSearchCompleteSuccessfully);

	// Function to call when join session is completed
	void OnJoinSessionComplete(FName SessionIn, EOnJoinSessionCompleteResult::Type SessionCompleteResult);

	void CreateSession() const;
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	TArray<FString> SessionResults;
	
	FString EnteredHostName = FString("");
};
