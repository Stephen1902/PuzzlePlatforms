// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainMenu.h"
#include "W_SessionInfo.generated.h"

/**
 * 
 */
UCLASS()
class SCIFIESCAPE_API UW_SessionInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionID;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionHost;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> tbSessionPlayers;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> btnSelectSession;

	// Default colour to display when a button is not hovered or clicked
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set up")
	FLinearColor DefaultColour;

	// Colour to display when button is hovered over but not clicked
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set up")
	FLinearColor HoveredColour;

	// Colour to display when button has been clicked
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set up")
	FLinearColor SelectedColour;

public:
	void Setup(class UW_MainMenu* Parent, uint32 Index);

	void SetSessionInfoText(FSessionInfoStruct SessionInfoIn) const;

	void SetColourToDefault();

private:
	UFUNCTION()
	void BtnSelectSessionClicked();

	UFUNCTION()
	void BtnSelectSessionHovered();

	UFUNCTION()
	void BtnSelectSessionUnhovered();

	TObjectPtr<UW_MainMenu> MainMenuRef;

	uint32 CurrentSessionInfoIndex;
	
	bool bIsSelectedSession = false;
};
