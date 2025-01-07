// Copyright Villains, Inc. All Rights Reserved.

#include "Features/UI/MenuSystem/MenuManager.h"
#include "Kismet/GameplayStatics.h"

void UMenuManager::OpenMenu(UMenuWidget* Menu)
{
	if (!Check())
		return;
	
	if (IsValid(CurrentOpenMenu))
		CloseMenu();

	Menu->Open();
	CurrentOpenMenu = Menu;
	SetMenuMappingContext();
}

void UMenuManager::CloseMenu()
{
	if (!Check())
		return;

	CurrentOpenMenu->Close();
	CurrentOpenMenu = nullptr;
	SetGameplayMappingContext();
}

void UMenuManager::ToggleMenu(UMenuWidget* Menu)
{
	if (!Check())
		return;

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

void UMenuManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (Check())
		SetGameplayMappingContext();
}

void UMenuManager::SetGameplayMappingContext() const
{
	EnhancedInputSubsystem->AddMappingContext(GameplayContext, 0);
	EnhancedInputSubsystem->RemoveMappingContext(MenuContext);
}

void UMenuManager::SetMenuMappingContext() const
{
	EnhancedInputSubsystem->AddMappingContext(MenuContext, 0);
	EnhancedInputSubsystem->RemoveMappingContext(GameplayContext);
}

bool UMenuManager::Check() const
{
	return IsValid(PlayerController) && IsValid(EnhancedInputSubsystem);
}
