// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class SCIFIESCAPE_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenPauseMenu(class UCPPGameInstance* GameInstanceIn);
	void ClosePauseMenu();

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	UWidget* PauseMenu;

	UPROPERTY(meta=(BindWidget))
	class UButton* btnQUIT;

	UPROPERTY(meta=(BindWidget))
	UButton* btnBACK;
	
	UPROPERTY(meta=(BindWidget))
	UWidget* QuitConfirmMenu;

	UPROPERTY(meta=(BindWidget))
	UButton* btnQuitDesktop;

	UPROPERTY(meta=(BindWidget))
	UButton* btnQuitMainMenu;

	UPROPERTY(meta=(BindWidget))
	UButton* btnQuitCancel;

private:
	// Functions for buttons on this widget
	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void QuitDesktopClicked();

	UFUNCTION()
	void QuitMainMenuClicked();

	UFUNCTION()
	void QuitCancelClicked();

	UPROPERTY()
	UCPPGameInstance* GameInstanceRef;
	
};
