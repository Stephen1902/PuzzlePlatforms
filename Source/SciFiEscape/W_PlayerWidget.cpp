// Copyright 2023 DME Games


#include "W_PlayerWidget.h"

#include "InteacteractiveActorBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

bool UW_PlayerWidget::Initialize()
{
	const bool Super = Super::Initialize();
	if (!Super) { return false; }

	TextBoxInteractInstructions->SetVisibility(ESlateVisibility::Hidden);
	ImageInteract->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UW_PlayerWidget::InteractiveItemFound(const AInteractiveActorBase* InteractiveActor) const
{
	if (InteractiveActor != nullptr)
	{
		TextBoxInteractInstructions->SetText(InteractiveActor->GetInteractiveInstructionText());
	}

	if (TextBoxInteractInstructions->GetVisibility() != ESlateVisibility::Visible)
	{
		TextBoxInteractInstructions->SetVisibility(ESlateVisibility::Visible);
		ImageInteract->SetVisibility(ESlateVisibility::Visible);
	}
}

void UW_PlayerWidget::InteractiveItemLost() const
{
	TextBoxInteractInstructions->SetVisibility(ESlateVisibility::Hidden);
	ImageInteract->SetVisibility(ESlateVisibility::Hidden);
}
