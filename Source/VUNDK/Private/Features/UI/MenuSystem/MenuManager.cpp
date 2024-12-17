// Copyright Villains, Inc. All Rights Reserved.

#include "Features/UI/MenuSystem/MenuManager.h"
#include "Kismet/GameplayStatics.h"

void UMenuManager::OpenMenu(UMenuWidget* Menu)
{
	if (!Check())
		return;
	
	if (IsValid(CurrentOpenMenu))
		CloseMenu(CurrentOpenMenu);

	Menu->Open();
	CurrentOpenMenu = Menu;
	SetMenuMappingContext();
}

void UMenuManager::CloseMenu(UMenuWidget* Menu)
{
	if (!Check())
		return;
	
	Menu->Close();
	CurrentOpenMenu = nullptr;
	SetGameplayMappingContext();
}

void UMenuManager::ToggleMenu(UMenuWidget* Menu)
{
	if (!Check())
		return;
	
	if (IsValid(CurrentOpenMenu))
		CloseMenu(CurrentOpenMenu);
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
