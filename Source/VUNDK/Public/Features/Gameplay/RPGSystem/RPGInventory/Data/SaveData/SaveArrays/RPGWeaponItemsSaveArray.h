// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "RPGWeaponItemsSaveArray.generated.h"

USTRUCT(BlueprintType)
struct FRPGWeaponItemsSaveArray
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRPGItemSaveData> WeaponItems;

	FRPGWeaponItemsSaveArray()
	{
		WeaponItems = TArray<FRPGItemSaveData>();
	}
};
