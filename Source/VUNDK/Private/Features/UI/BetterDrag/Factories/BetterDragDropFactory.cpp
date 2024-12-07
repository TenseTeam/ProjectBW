// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features\UI\BetterDrag\Factories\BetterDragDropOperationFactory.h"

UBetterDragDropOperation* UBetterDragAndDropFactory::CreateBetterDragDropOperation(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, const FKey DropKey)
{
	UBetterDragDropOperation* DragDropOperation = NewObject<UBetterDragDropOperation>(OwnerWidget, DragDropOperationClass);
	DragDropOperation->Init(OwnerWidget, DragVisual, Payload, OwnerWidgetVisibility, DropKey);
	return DragDropOperation;
}
