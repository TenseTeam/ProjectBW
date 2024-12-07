// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveArrays/RPGGearItemsSaveArray.h"
#include "SaveArrays/RPGWeaponItemsSaveArray.h"
#include "RPGInventoryItemsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRPGInventoryItemsSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FRPGGearItemsSaveArray> GearItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FRPGWeaponItemsSaveArray> WeaponItems;

	FRPGInventoryItemsSaveData()
	{
		GearItems = TMap<FName, FRPGGearItemsSaveArray>();
		WeaponItems = TMap<FName, FRPGWeaponItemsSaveArray>();
	}
};
