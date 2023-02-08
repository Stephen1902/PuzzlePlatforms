// Copyright 2023 DME Games


#include "W_MainMenu.h"
#include "Components/Button.h"

bool UW_MainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (btnHOST)
	{
		btnHOST->OnClicked.AddDynamic(this, &UW_MainMenu::HostButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnJOIN)
	{
		btnJOIN->OnClicked.AddDynamic(this, &UW_MainMenu::JoinButtonClicked);
	}
	else
	{
		Success = false;
	}

	if (btnQUIT)
	{
		btnQUIT->OnClicked.AddDynamic(this, &UW_MainMenu::QuitButtonClicked);
	}
	else
	{
		Success = false;
	}

	
	return Success;
}

void UW_MainMenu::HostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Button Clicked"));
}

void UW_MainMenu::JoinButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Button Clicked"));
}

void UW_MainMenu::QuitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked"));
}
