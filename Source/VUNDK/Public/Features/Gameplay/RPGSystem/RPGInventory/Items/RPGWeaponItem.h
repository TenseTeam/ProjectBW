// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGWeaponItemData.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/StatOperation.h"
#include "RPGWeaponItem.generated.h"

UCLASS()
class VUNDK_API URPGWeaponItem : public URPGItemBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UStatOperation* WeaponDamageOperation;
	
public:
	void LoadRPGWeaponItemSaveData(const FRPGItemSaveData& RPGItemSaveData, URPGInventory* Inventory);
	
	UFUNCTION(BlueprintPure)
	int32 GetRollWeaponDamage() const;
	
	UFUNCTION(BlueprintPure)
	URPGWeaponItemData* GetRPGWeaponData() const;
};
