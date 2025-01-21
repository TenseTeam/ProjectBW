// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "WeaponItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGGearItemSaveData GearItemSaveData;

	FWeaponItemSaveData() : GearItemSaveData()
	{
	}
};
