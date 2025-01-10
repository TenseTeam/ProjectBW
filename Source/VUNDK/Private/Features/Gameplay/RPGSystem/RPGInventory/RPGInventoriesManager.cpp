// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"

URPGInventoriesManager::URPGInventoriesManager(): ItemsStats(nullptr),
                                                  ItemsRarities(nullptr)
{
}

URPGRarityLevelData* URPGInventoriesManager::GetRarityByID(const FName RarityID) const
{
	for (URPGRarityLevelData* RarityLevel : ItemsRarities->RarityLevels)
	{
		if (RarityLevel->RarityID.IsEqual(RarityID))
			return RarityLevel;
	}

	return nullptr;
}

UCoreStatData* URPGInventoriesManager::GetStatByID(const FName StatID) const
{
	if (!ItemsStats)
		return nullptr;
	
	for (UCoreStatData* Stat : ItemsStats->Stats)
	{
		if (Stat->StatID.IsEqual(StatID))
			return Stat;
	}
	
	return nullptr;
}
