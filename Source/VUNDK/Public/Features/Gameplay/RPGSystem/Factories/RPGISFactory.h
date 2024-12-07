// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGWeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGItemDataBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGWeaponItemData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGGearItemGenerationData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGISFactory.generated.h"

UCLASS()
class VUNDK_API URPGISFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static URPGItemBase* CreateRPGItemBase(URPGItemDataBase* Data,  const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem = true);

	UFUNCTION(BlueprintCallable, Category = "RPG Inventory Factory")
	static URPGWeaponItem* CreateRPGWeaponItem(URPGWeaponItemData* Data,  const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem = true);
	
	UFUNCTION(BlueprintCallable, Category = "RPG Inventory Factory")
	static URPGGearItem* CreateRPGGearItem(URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem = true);
};
