// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/RPGGearItemGenerationData.h"
#include "Data/Rarities/RPGItemsRaritiesData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGItemsGenerator.generated.h"

UCLASS()
class VUNDK_API URPGItemsGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|RPGSystem")
	static bool TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData);
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|RPGSystem")
	static bool TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData);

private:
	static FRPGItemVisualDetails GenerateVisualDetails(const URPGItemVisualSetData* Set);
	
	static URPGRarityLevelData* GenerateRarityLevel(const URPGItemsRaritiesData* Set);
	
	static void GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData);
};
