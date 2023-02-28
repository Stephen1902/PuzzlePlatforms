// Copyright 2023 DME Games


#include "W_SessionInfo.h"

#include "W_MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UW_SessionInfo::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) { return false;}

	if (btnSelectSession)
	{
		btnSelectSession->OnClicked.AddDynamic(this, &UW_SessionInfo::BtnSelectSessionClicked);
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

void UW_SessionInfo::BtnSelectSessionClicked()
{
	// Check we successfully got the main menu class from the code in the Setup function
	if (!ensure(MainMenuRef != nullptr)) return;

	MainMenuRef->SelectSessionIndex(CurrentSessionInfoIndex);
}
