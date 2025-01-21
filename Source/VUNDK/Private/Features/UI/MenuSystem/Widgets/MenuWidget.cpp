// Copyright Villains, Inc. All Rights Reserved.

#include "Features/UI/MenuSystem/Widgets/MenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = GetOwningPlayer();
	Close();
	AddToViewport(ZOrder);
}

void UMenuWidget::Open()
{
	if (!Check())
		return;
	
	if (bIsOpen)
		return;

	SetVisibility(OpenVisibility);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, this, EMouseLockMode::DoNotLock, false, true);
	PlayerController->bShowMouseCursor = true;
	bIsOpen = true;
	OnOpen();
}

void UMenuWidget::Close()
{
	if (!Check())
		return;
	
	if (!bIsOpen)
		return;
	
	SetVisibility(CloseVisibility);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);
	PlayerController->bShowMouseCursor = false;
	bIsOpen = false;
	OnClose();
}

void UMenuWidget::OnClose_Implementation()
{
}

void UMenuWidget::OnOpen_Implementation()
{
}

bool UMenuWidget::Check() const
{
	return IsValid(PlayerController);
}
