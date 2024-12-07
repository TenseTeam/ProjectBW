// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/UI/BetterDrag/BetterDragDropManager.h"

UBetterDragDropOperation* UBetterDragDropManager::CurrentOperation = nullptr;

void UBetterDragDropManager::SetDragDropOperation(UBetterDragDropOperation* InOperation)
{
	if (InOperation == nullptr) return;
	
	CurrentOperation = InOperation;
}

void UBetterDragDropManager::ClearDragDropOperation()
{
	CurrentOperation = nullptr;
}

void UBetterDragDropManager::EnterReceiver(UUserWidget* InDropReceiver)
{
	if (CurrentOperation == nullptr) return;
	
	CurrentOperation->EnterReceiver(InDropReceiver);
}

void UBetterDragDropManager::LeaveReceiver()
{
	if (CurrentOperation == nullptr) return;
	
	CurrentOperation->LeaveReceiver();
}
