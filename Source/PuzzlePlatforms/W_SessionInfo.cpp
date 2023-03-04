// Copyright 2023 DME Games


#include "W_SessionInfo.h"

#include "W_MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UW_SessionInfo::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) { return false;}

	if (btnSelectSession && tbSessionInfo)
	{
		btnSelectSession->OnClicked.AddDynamic(this, &UW_SessionInfo::BtnSelectSessionClicked);
		btnSelectSession->OnHovered.AddDynamic(this, &UW_SessionInfo::BtnSelectSessionHovered);
		btnSelectSession->OnUnhovered.AddDynamic(this, &UW_SessionInfo::BtnSelectSessionUnhovered);
	}
	else
	{
		bSuccess = false;
	}
	
	return bSuccess;
}

void UW_SessionInfo::SetSessionInfoText(const FText TextToDisplay) const
{
	if (tbSessionInfo)
	{
		tbSessionInfo->SetText(TextToDisplay);
	}
}

void UW_SessionInfo::Setup(UW_MainMenu* Parent, uint32 Index)
{
	MainMenuRef = Parent;
	CurrentSessionInfoIndex = Index;
}

void UW_SessionInfo::SetColourToDefault()
{
	if (tbSessionInfo)
	{
		tbSessionInfo->SetColorAndOpacity(DefaultColour);
		bIsSelectedSession = false;
	}
}

void UW_SessionInfo::BtnSelectSessionClicked()
{
	// Check we successfully got the main menu class from the code in the Setup function
	if (!ensure(MainMenuRef != nullptr)) return;

	MainMenuRef->SelectSessionIndex(CurrentSessionInfoIndex);

	if (tbSessionInfo)
	{
		tbSessionInfo->SetColorAndOpacity(SelectedColour);
		bIsSelectedSession = true;
	}
}

void UW_SessionInfo::BtnSelectSessionHovered()
{
	if (tbSessionInfo && !bIsSelectedSession)
	{
		tbSessionInfo->SetColorAndOpacity(HoveredColour);
	}
}

void UW_SessionInfo::BtnSelectSessionUnhovered()
{
	if (tbSessionInfo && !bIsSelectedSession)
	{
		tbSessionInfo->SetColorAndOpacity(DefaultColour);
	}
}
