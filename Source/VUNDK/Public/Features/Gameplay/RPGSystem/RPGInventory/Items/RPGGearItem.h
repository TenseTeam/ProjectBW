// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemBase.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Data/CharacterBaseStatData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "RPGGearItem.generated.h"

class URPGGearItemData;

UCLASS()
class VUNDK_API URPGGearItem : public URPGItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<UCharacterBaseStatData*, int32> StatsModifiers;
	UPROPERTY(BlueprintReadOnly)
	UCharacterBaseStatData* MainStat = nullptr;

public:
	URPGGearItem();
	
	FRPGGearItemSaveData CreateRPGGearItemSaveData() const;
	
	void LoadRPGGearItemSaveData(FRPGGearItemSaveData& GearSaveData, URPGInventory* Inventory);
	
	UFUNCTION(BlueprintPure)
	URPGGearItemData* GetRPGGearItemData() const;
};
