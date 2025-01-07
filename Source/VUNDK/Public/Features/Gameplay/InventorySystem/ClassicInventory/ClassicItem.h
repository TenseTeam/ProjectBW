// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/ClassicItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "UObject/Object.h"
#include "ClassicItem.generated.h"

class UClassicItemData;
struct FClassicItemSlotData;

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API UClassicItem : public UItemBase
{
	GENERATED_BODY()

public:
	FClassicItemSaveData CreateClassicItemSaveData() const;

	void LoadClassicItemSaveData(const FClassicItemSaveData ItemSaveData);

	UFUNCTION(BlueprintPure)
	UClassicItemData* GetClassicItemData() const;
	
	UFUNCTION(BlueprintPure)
	TArray<FClassicItemSlotData> GetRequiredSlots() const;

	UFUNCTION(BlueprintPure)
	int32 GetMinRequiredSlots() const;
};
