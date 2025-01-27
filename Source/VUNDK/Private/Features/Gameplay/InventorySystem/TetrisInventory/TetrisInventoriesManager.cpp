// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventoriesManager.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"

UTetrisInventoriesManager::UTetrisInventoriesManager()
{
}

bool UTetrisInventoriesManager::TryTransferItem(UTetrisItem* Item, UTetrisInventory* SourceInventory, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition)
{
	if (Item == nullptr || DestinationInventory == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item, DestinationInventory is not valid!"));
		return false;
	}

	if (SourceInventory == DestinationInventory)
		return SourceInventory->TryMoveItem(Item, DestinationPosition);

	if (DestinationInventory->CanOccupySlots(Item->GetRelativeSize(), DestinationPosition))
	{
		Item->Remove();
		DestinationInventory->TryAddItemAtSlots(Item, DestinationPosition);
		return true;
	}

	return false;
}
