// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGRarityLevelData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGInventoriesUtility.generated.h"

UCLASS()
class VUNDK_API URPGInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	static URPGRarityLevelData* GetRarityByID(const FName RarityID);

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	static URPGInventoriesManager* GetRPGInventoriesManager();
};
