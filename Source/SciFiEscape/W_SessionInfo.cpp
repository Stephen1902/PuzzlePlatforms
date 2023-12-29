// Copyright 2023 DME Games


#include "W_SessionInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UW_SessionInfo::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) { return false;}

	if (btnSelectSession && tbSessionID &&tbSessionHost && tbSessionPlayers)
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

void UW_SessionInfo::SetSessionInfoText(FSessionInfoStruct SessionInfoIn) const
{
	if (tbSessionID && tbSessionHost && tbSessionPlayers)
	{
		tbSessionID->SetText(FText::FromString(SessionInfoIn.SessionName));
		tbSessionHost->SetText(FText::FromString(SessionInfoIn.HostUserName));
		const FString PlayerDisplay = FString::FromInt(SessionInfoIn.CurrentPlayers) + "/" + FString::FromInt(SessionInfoIn.MaxPlayers);
		tbSessionPlayers->SetText(FText::FromString(PlayerDisplay));
	}
}

void UW_SessionInfo::Setup(UW_MainMenu* Parent, uint32 Index)
{
	MainMenuRef = Parent;
	CurrentSessionInfoIndex = Index;
}

void UW_SessionInfo::SetColourToDefault()
{
	if (tbSessionID)
	{
		tbSessionID->SetColorAndOpacity(DefaultColour);
		tbSessionHost->SetColorAndOpacity(DefaultColour);
		tbSessionPlayers->SetColorAndOpacity(DefaultColour);
		bIsSelectedSession = false;
	}
}

void UW_SessionInfo::BtnSelectSessionClicked()
{
	// Check we successfully got the main menu class from the code in the Setup function
	if (!ensure(MainMenuRef != nullptr)) return;

	MainMenuRef->SelectSessionIndex(CurrentSessionInfoIndex);

	if (tbSessionID)
	{
		tbSessionID->SetColorAndOpacity(SelectedColour);
		tbSessionHost->SetColorAndOpacity(SelectedColour);
		tbSessionPlayers->SetColorAndOpacity(SelectedColour);
		bIsSelectedSession = true;
	}
}

void UW_SessionInfo::BtnSelectSessionHovered()
{
	if (tbSessionID && !bIsSelectedSession)
	{
		tbSessionID->SetColorAndOpacity(HoveredColour);
		tbSessionHost->SetColorAndOpacity(HoveredColour);
		tbSessionPlayers->SetColorAndOpacity(HoveredColour);
	}
}

void UW_SessionInfo::BtnSelectSessionUnhovered()
{
	if (tbSessionID && !bIsSelectedSession)
	{
		tbSessionID->SetColorAndOpacity(DefaultColour);
		tbSessionHost->SetColorAndOpacity(DefaultColour);
		tbSessionPlayers->SetColorAndOpacity(DefaultColour);
	}
}
