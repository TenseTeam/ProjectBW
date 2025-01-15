// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/CoreStatData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Containers/CoreStatsContainer.h"
#include "RPGGearItem.generated.h"

class URPGItemStatOperation;
class URPGGearItemData;

constexpr int32 MinValidStatValue = -0.05f;
constexpr int32 MaxValidStatValue = 0.05f;

UCLASS()
class VUNDK_API URPGGearItem : public URPGItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UCoreStatsContainer* GearStatsContainer;
	
public:
	URPGGearItem();

	virtual void Init(UItemDataBase* Data) override;
	
	FRPGGearItemSaveData CreateRPGGearItemSaveData() const;
	
	void LoadRPGGearItemSaveData(URPGInventory* LoadingInventory, FRPGGearItemSaveData& GearSaveData);
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "bOverrideIfExist"))
	void AddItemStat(UCoreStatData* Stat, TSubclassOf<URPGItemStatOperation> OperationClass);
	
	UFUNCTION(BlueprintPure)
	URPGGearItemData* GetRPGGearItemData() const;

	virtual bool CanStackItem_Implementation(UItemBase* OtherItem) const override;
};
