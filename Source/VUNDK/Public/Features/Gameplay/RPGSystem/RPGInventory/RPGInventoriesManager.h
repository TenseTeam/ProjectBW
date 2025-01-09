// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventoriesManager.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/ItemsStatsData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/CoreStatData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/Rarities/RPGItemsRaritiesData.h"
#include "RPGInventoriesManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API URPGInventoriesManager : public UTetrisInventoriesManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UItemsStatsData* ItemsStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPGItemsRaritiesData* ItemsRarities;
	
public:
	URPGInventoriesManager();

	UFUNCTION(BlueprintPure)
	URPGRarityLevelData* GetRarityByID(const FName RarityID) const;

	UFUNCTION(BlueprintPure)
	UCoreStatData* GetStatByID(const FName StatID) const;
};
