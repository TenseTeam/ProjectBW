// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/RPGGearItemGenerationData.h"
#include "Data/RPGItemsRaritiesData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGWeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGMainStatsData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGItemsGenerator.generated.h"

UCLASS()
class VUNDK_API URPGItemsGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool TryGenerateRPGItemBase(URPGItemBase* Item, const URPGItemBaseGenerationData* GenerationData);

	UFUNCTION(BlueprintCallable, Category = "RPG Inventory Factory")
	static bool TryGenerateRPGWeaponItem(URPGWeaponItem* WeaponItem, const URPGItemBaseGenerationData* GenerationData);
	
	UFUNCTION(BlueprintCallable, Category = "RPG Inventory Factory")
	static bool TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData);

	static UStatOperation* CreateWeaponDamageOperation(URPGWeaponItem* WeaponItem);
	
private:
	static FRPGItemVisualDetails GenerateVisualDetails(const URPGItemVisualSetData* Set);
	
	static URPGRarityLevelData* GenerateRarityLevel(const URPGItemsRaritiesData* Set);
	
	static TMap<UCharacterBaseStatData*, int32> GenerateStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData);
	
	static UCharacterBaseStatData* GetRandomMainStat(const URPGMainStatsData* MainStatsPool);
};
