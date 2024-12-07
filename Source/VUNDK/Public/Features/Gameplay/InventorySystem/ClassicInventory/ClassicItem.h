// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/ClassicItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "UObject/Object.h"
#include "ClassicItem.generated.h"

class UClassicItemData;
struct FClassicItemSlotData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemIncreased,
	int32, CurrentQuantity
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemDecreased,
	int32, CurrentQuantity
);

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API UClassicItem : public UItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemIncreased OnItemIncreased;
	UPROPERTY(BlueprintAssignable)
	FOnItemDecreased OnItemDecreased;

private:
	int32 CurrentQuantity;

public:
	void OnInit_Implementation() override;

	FClassicItemSaveData CreateClassicItemSaveData() const;

	void LoadClassicItemSaveData(const FClassicItemSaveData ItemSaveData);

	UFUNCTION(BlueprintPure)
	UClassicItemData* GetClassicItemData() const;
	
	UFUNCTION(BlueprintPure)
	TArray<FClassicItemSlotData> GetRequiredSlots() const;

	UFUNCTION(BlueprintPure)
	int32 GetMinRequiredSlots() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxStackSize() const;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentQuantity() const;
	
	void IncreaseQuantity();

	void DecreaseQuantity();

protected:
	virtual void OnConsume_Implementation() override;
};
