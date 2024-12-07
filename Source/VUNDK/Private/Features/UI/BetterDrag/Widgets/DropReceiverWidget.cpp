// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/UI/BetterDrag/Widgets/DropReceiverWidget.h"
#include "Features/UI/BetterDrag/BetterDragDropManager.h"

void UDropReceiverWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UBetterDragDropManager::EnterReceiver(this);
}

void UDropReceiverWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UBetterDragDropManager::LeaveReceiver();
}
