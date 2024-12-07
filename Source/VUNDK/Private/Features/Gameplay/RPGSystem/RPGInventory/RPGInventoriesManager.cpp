// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"

URPGInventoriesManager::URPGInventoriesManager()
{
	ItemsRarities = nullptr;
}

URPGRarityLevelData* URPGInventoriesManager::GetRarityByID(const FName RarityID) const
{
	for (URPGRarityLevelData* RarityLevel : ItemsRarities->RarityLevels)
	{
		if (RarityLevel->RarityID == RarityID)
		{
			return RarityLevel;
		}
	}

	return nullptr;
}
