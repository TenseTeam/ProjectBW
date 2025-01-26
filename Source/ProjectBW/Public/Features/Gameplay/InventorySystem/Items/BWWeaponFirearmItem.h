// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BWWeaponItem.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/WeaponFirearmSaveData.h"
#include "BWWeaponFirearmItem.generated.h"

UCLASS()
class PROJECTBW_API UBWWeaponFirearmItem : public UBWWeaponItem
{
	GENERATED_BODY()

protected:
	int32 AmmoRemaining;
	bool bHasNewMag = true;
	
public:
	FWeaponFirearmSaveData CreateWeaponFirearmSaveData() const;

	void LoadWeaponFirearmItemSaveData(UBWInventory* LoadingInventory, FWeaponFirearmSaveData& WeaponItemSaveData);
	
	void SetAmmoRemaining(const int32 NewAmmoRemaining);

	bool TryConsumeWeaponItemNewMagazine(const int MagSize);
	
	int32 GetAmmoRemaining() const;
	
	UFUNCTION(BlueprintNativeEvent)
	float GetFireRate();
	
	UFUNCTION(BlueprintNativeEvent)
	float GetMaxRange();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetMagSize();
};
