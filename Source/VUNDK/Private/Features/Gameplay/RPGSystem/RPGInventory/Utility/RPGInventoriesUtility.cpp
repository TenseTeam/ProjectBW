// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UBaseStatData* URPGInventoriesUtility::GetStatByID(const FName StatID)
{
	return GetRPGInventoriesManager()->GetStatByID(StatID);
}

URPGRarityLevelData* URPGInventoriesUtility::GetRarityByID(const FName RarityID)
{
	return GetRPGInventoriesManager()->GetRarityByID(RarityID);
}

URPGInventoriesManager* URPGInventoriesUtility::GetRPGInventoriesManager()
{
	return Cast<URPGInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
}
