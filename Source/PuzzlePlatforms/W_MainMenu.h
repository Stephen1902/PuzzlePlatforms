// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "W_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()

	UW_MainMenu(const FObjectInitializer& ObjectInitializer);

public:
	void SetGameInstance(class UCPPGameInstance* GameInstanceIn);

	void SetupMenu();
	void UpdateSessionList(TArray<FOnlineSessionSearchResult> SessionSearchResultsIn);

	void SelectSessionIndex(uint32 IndexIn);
	
protected:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	UWidget* MainMenu;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* btnHOST;

	UPROPERTY(meta=(BindWidget))
	UButton* btnJOIN;

	UPROPERTY(meta=(BindWidget))
	UButton* btnQUIT;

	UPROPERTY(meta=(BindWidget))
	UWidget* LoginMenu;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionSearchInfo;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UScrollBox> sbSessionInfo;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> btnJoinServerRefresh;
	
	UPROPERTY(meta=(BindWidget))
	UButton* btnJoinServerOK;

	UPROPERTY(meta=(BindWidget))
	UButton* btnJoinServerCancel;

	UPROPERTY(meta=(BindWidget))
	UWidget* QuitMenu;
	
	UPROPERTY(meta=(BindWidget))
	UButton* btnQuitGameYes;

	UPROPERTY(meta=(BindWidget))
	UButton* btnQuitGameNo;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UFUNCTION()
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HostButtonClicked();

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

	TOptional<uint32> SelectedSessionIndex;

};
