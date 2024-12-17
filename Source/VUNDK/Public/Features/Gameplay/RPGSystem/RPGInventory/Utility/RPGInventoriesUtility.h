// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGInventoriesUtility.generated.h"

UCLASS()
class VUNDK_API URPGInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|RPGSystem")
	static UBaseStatData* GetStatByID(const FName StatID);
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|RPGSystem")
	static URPGRarityLevelData* GetRarityByID(const FName RarityID);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|RPGSystem")
	static URPGInventoriesManager* GetRPGInventoriesManager();
};
