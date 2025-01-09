// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/ItemStatOperation.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGGearItemGenerationData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SpecialStatData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGFactory.generated.h"

UCLASS()
class VUNDK_API URPGFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bBuildItem"), Category = "VUNDK|Gameplay|RPGSystem")
	static URPGItem* CreateRPGItem(URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem = true, const int32 Quantity = 1);
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bBuildItem"), Category = "VUNDK|Gameplay|RPGSystem")
	static URPGGearItem* CreateRPGGearItem(URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem = true);
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|RPGSystem")
	static UItemStatOperation* CreateItemStatOperation(const TSubclassOf<UItemStatOperation> OperationClass, URPGItem* Item);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|RPGSystem")
	static UStatOperation* CreateBridgeStatOperation(UStatsBridgeBase* Bridge, USpecialStatData* SpecialStatData);
};
