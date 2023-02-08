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

protected:
	virtual bool Initialize() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* btnHOST;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* btnJOIN;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* btnQUIT;

private:
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();
};
