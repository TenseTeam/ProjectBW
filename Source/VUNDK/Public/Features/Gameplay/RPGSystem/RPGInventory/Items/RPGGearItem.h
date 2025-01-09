// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/BaseStatData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "RPGGearItem.generated.h"

class UItemStatOperation;
class URPGGearItemData;

UCLASS()
class VUNDK_API URPGGearItem : public URPGItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<UBaseStatData*, int32> GearStats;

public:
	URPGGearItem();
	
	FRPGGearItemSaveData CreateRPGGearItemSaveData() const;
	
	void LoadRPGGearItemSaveData(URPGInventory* LoadingInventory, FRPGGearItemSaveData& GearSaveData);

	UFUNCTION(BlueprintPure)
	int32 GetItemStatValue(const UBaseStatData* Stat) const;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bOverrideIfExist"))
	void AddItemStat(UBaseStatData* Stat, TSubclassOf<UItemStatOperation> OperationClass, bool bOverrideIfExist = false);

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
