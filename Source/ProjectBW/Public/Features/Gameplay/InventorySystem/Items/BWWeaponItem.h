// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/BWInventory.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/WeaponItemSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "BWWeaponItem.generated.h"

class UWeaponItemData;

UCLASS()
class PROJECTBW_API UBWWeaponItem : public URPGGearItem
{
	GENERATED_BODY()

public:
	FWeaponItemSaveData CreateWeaponItemSaveData() const;

	void LoadWeaponItemSaveData(UBWInventory* LoadingInventory, FWeaponItemSaveData& WeaponItemSaveData);
	
	UFUNCTION(BlueprintPure)
	UWeaponItemData* GetWeaponItemData() const;

	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponDamage();
};
