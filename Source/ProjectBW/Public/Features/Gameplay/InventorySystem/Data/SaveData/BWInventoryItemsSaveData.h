// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponFirearmItemsSaveArray.h"
#include "WeaponItemsSaveArray.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventoryItemsSaveData.h"
#include "BWInventoryItemsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FBWInventoryItemsSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGInventoryItemsSaveData RPGItemsSaveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FWeaponItemsSaveArray> GenericWeaponItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FWeaponFirearmItemsSaveArray> WeaponFirearmItems;
};
