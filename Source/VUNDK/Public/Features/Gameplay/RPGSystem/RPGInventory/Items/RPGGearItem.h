// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/BaseStatData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "RPGGearItem.generated.h"

class URPGGearItemData;

UCLASS()
class VUNDK_API URPGGearItem : public URPGItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<UBaseStatData*, int32> StatsModifiers;

public:
	URPGGearItem();
	
	FRPGGearItemSaveData CreateRPGGearItemSaveData() const;
	
	void LoadRPGGearItemSaveData(FRPGGearItemSaveData& GearSaveData, URPGInventory* Inventory);

	UFUNCTION(BlueprintPure)
	int32 GetItemStatValue(const UBaseStatData* Stat) const;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bOverrideIfExist"))
	void AddItemStat(UBaseStatData* Stat, TSubclassOf<UStatOperation> OperationClass, bool bOverrideIfExist = false);

	UFUNCTION(BlueprintCallable)
	void RemoveItemStat(UBaseStatData* Stat);
	
	UFUNCTION(BlueprintCallable)
	void SetItemStat(UBaseStatData* Stat, int32 Value);

	UFUNCTION(BlueprintCallable) 
	void ModifyItemStat(UBaseStatData* Stat, int32 SumValue);
	
	UFUNCTION(BlueprintPure)
	URPGGearItemData* GetRPGGearItemData() const;

	virtual bool CanStackItem_Implementation(UItemBase* OtherItem) const override;
};
