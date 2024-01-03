// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCIFIESCAPE_API UW_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImageInteract;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextBoxInteractInstructions;

public:
	void InteractiveItemFound(const class AInteractiveActorBase* InteractiveActor) const;
	void InteractiveItemLost() const;
	
};
