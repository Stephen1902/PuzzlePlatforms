// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SessionInfo.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UW_SessionInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionInfo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> btnSelectSession;

public:
	void SetSessionInfoText(FText TextToDisplay) const;

	void Setup(class UW_MainMenu* Parent, uint32 Index);

private:
	UFUNCTION()
	void BtnSelectSessionClicked();

	TObjectPtr<UW_MainMenu> MainMenuRef;

	uint32 CurrentSessionInfoIndex;

	
	
	
};
