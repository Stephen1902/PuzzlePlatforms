// Copyright 2023 DME Games


#include "W_PlayerWidget.h"

#include "Components/Image.h"

bool UW_PlayerWidget::Initialize()
{
	const bool Super = Super::Initialize();
	if (!Super) { return false; }
		
	ImageInteract->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UW_PlayerWidget::InteractiveItemFound() const
{
	ImageInteract->SetVisibility(ESlateVisibility::Visible);
}

void UW_PlayerWidget::InteractiveItemLost() const
{
	ImageInteract->SetVisibility(ESlateVisibility::Hidden);
}
