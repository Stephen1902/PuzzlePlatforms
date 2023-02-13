// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGameInstance(class UCPPGameInstance* GameInstanceIn);

	void SetupMenu();
	void RemoveMenu();
	
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

	UPROPERTY(meta=(BindWidget))
	class UEditableText* tbAddressEntry;

	UFUNCTION()
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

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
};
