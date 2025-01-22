// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/WeaponFirearmSaveData.h"
#include "WeaponFirearmItem.generated.h"

UCLASS()
class PROJECTBW_API UWeaponFirearmItem : public UWeaponItem
{
	GENERATED_BODY()

protected:
	int32 AmmoRemaining;
	
public:
	FWeaponFirearmSaveData CreateWeaponFirearmSaveData() const;

	void LoadWeaponFirearmItemSaveData(UBWInventory* LoadingInventory, FWeaponFirearmSaveData& WeaponItemSaveData);

	UFUNCTION(BlueprintCallable)
	void SetAmmoRemaining(const int32 NewAmmoRemaining);

	UFUNCTION(BlueprintPure)
	int32 GetAmmoRemaining() const;
	
	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponFireRate();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetWeaponMagSize();

	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponRange();
};
