// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/WeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/WeaponSystem/WeaponBase.h"
#include "WeaponItemData.generated.h"

UCLASS()
class PROJECTBW_API UWeaponItemData : public URPGGearItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> WeaponClass;
	
public:
	UWeaponItemData()
	{
		ItemClass = UWeaponItem::StaticClass();
	}
};
