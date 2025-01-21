// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UCoreStatData* URPGInventoriesUtility::GetStatByID(const FGuid StatID)
{
	return GetRPGInventoriesManager()->GetStatByID(StatID);
}

URPGRarityLevelData* URPGInventoriesUtility::GetRarityByID(const FGuid RarityID)
{
	return GetRPGInventoriesManager()->GetRarityByID(RarityID);
}

URPGInventoriesManager* URPGInventoriesUtility::GetRPGInventoriesManager()
{
	return Cast<URPGInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
}
