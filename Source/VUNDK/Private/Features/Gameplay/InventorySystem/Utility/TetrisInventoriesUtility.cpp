// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Utility/TetrisInventoriesUtility.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UTetrisInventoriesManager* UTetrisInventoriesUtility::GetTetrisInventoriesManager()
{
	return Cast<UTetrisInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
}

bool UTetrisInventoriesUtility::TryTransferItem(UTetrisItem* Item, UTetrisInventory* SourceInventory, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition)
{
	return GetTetrisInventoriesManager()->TryTransferItem(Item, SourceInventory, DestinationInventory, DestinationPosition);
}
