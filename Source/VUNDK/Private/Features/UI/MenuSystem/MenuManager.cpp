// Copyright Villains, Inc. All Rights Reserved.

#include "Features/UI/MenuSystem/MenuManager.h"
#include "Features/Generic/IMCManagerSystem/ContextsManagerUtility.h"

void UMenuManager::OpenMenu(UMenuWidget* Menu)
{
	if (IsValid(CurrentOpenMenu))
		CloseMenu();

	Menu->Open();
	CurrentOpenMenu = Menu;

	if (bSwitchMappingContexts)
		UContextsManagerUtility::SetUIMappingContext();
}

void UMenuManager::CloseMenu()
{
	CurrentOpenMenu->Close();
	CurrentOpenMenu = nullptr;

	if (bSwitchMappingContexts)
		UContextsManagerUtility::SetGameplayMappingContext();
}

void UMenuManager::ToggleMenu(UMenuWidget* Menu)
{
	if (CurrentOpenMenu == Menu)
	{
		CloseMenu();
		return;
	}
	
	if (IsValid(CurrentOpenMenu))
		CloseMenu();
	else
		OpenMenu(Menu);
}

UMenuWidget* UMenuManager::GetCurrentOpenMenu() const
{
	return CurrentOpenMenu;
}
