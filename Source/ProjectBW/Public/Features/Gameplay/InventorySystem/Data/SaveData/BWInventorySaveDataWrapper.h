// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponFirearmItemsSaveArray.h"
#include "WeaponItemsSaveArray.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventoryItemsSaveData.h"
#include "BWInventorySaveDataWrapper.generated.h"

USTRUCT(BlueprintType)
struct FBWInventorySaveDataWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGInventoryItemsSaveData RPGItemsSaveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FWeaponItemsSaveArray> GenericWeaponItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FWeaponFirearmItemsSaveArray> WeaponFirearmItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, int32> AmmoSaveData;
};
