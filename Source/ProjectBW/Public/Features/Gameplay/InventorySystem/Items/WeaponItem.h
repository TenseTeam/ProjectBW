// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "WeaponItem.generated.h"

class UWeaponItemData;

UCLASS()
class PROJECTBW_API UWeaponItem : public URPGGearItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UWeaponItemData* GetWeaponItemData() const;

	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponDamage();
};
