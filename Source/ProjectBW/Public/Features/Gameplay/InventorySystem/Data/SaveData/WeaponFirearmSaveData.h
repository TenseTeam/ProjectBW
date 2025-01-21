// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItemSaveData.h"
#include "WeaponFirearmSaveData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponFirearmSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponItemSaveData WeaponItemSaveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoRemaining;

	FWeaponFirearmSaveData() : WeaponItemSaveData()
	{
	}
};
