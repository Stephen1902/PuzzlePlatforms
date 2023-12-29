// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainMenu.generated.h"

USTRUCT()
struct FSessionInfoStruct
{
	GENERATED_BODY()

	// Session name for this session
	FString SessionName;

	// Current players in this session
	uint16 CurrentPlayers;

	// Total players who can join this session
	uint16 MaxPlayers;

	// The name of the host of this server
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class SCIFIESCAPE_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()

	UW_MainMenu(const FObjectInitializer& ObjectInitializer);

public:
	void SetGameInstance(class UCPPGameInstance* GameInstanceIn);

	void SetupMenu();
	void UpdateSessionList(TArray<FSessionInfoStruct> SessionSearchResultsIn);
	
	void SelectSessionIndex(uint32 IndexIn);
	
protected:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidget> MainMenu;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> btnHOST;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnJOIN;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnQUIT;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidget> HostMenu;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UEditableText> tbHostNameEntry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnHostNameOK;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnHostNameCancel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidget> LoginMenu;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionSearchInfo;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UScrollBox> sbSessionInfo;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnJoinServerRefresh;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnJoinServerOK;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnJoinServerCancel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidget> QuitMenu;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnQuitGameYes;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnQuitGameNo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UFUNCTION()
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void OnHostTextChanged(const FText& InText);
	
	UFUNCTION()
	void HostNameOKClicked();

	UFUNCTION()
	void HostNameCancelClicked();

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	void JoinButtonRefreshClicked();
	
	UFUNCTION()
	void JoinButtonOKClicked();

	UFUNCTION()
	void JoinButtonCancelClicked();

	UFUNCTION()
	void QuitGameYesClicked();

	UFUNCTION()
	void QuitGameNoClicked();

	UFUNCTION()
	void QuitButtonClicked();

	UPROPERTY()
	UCPPGameInstance* GameInstanceRef;

	UPROPERTY()
	TObjectPtr<class UW_SessionInfo> SessionInfoRef;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> SessionInfoClass;

	TOptional<uint32> SessionIndexSelected;

};
