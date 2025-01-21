// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponFirearmSaveData.h"
#include "WeaponFirearmItemsSaveArray.generated.h"

USTRUCT(BlueprintType)
struct FWeaponFirearmItemsSaveArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponFirearmSaveData> WeaponFirearmItems;

	FWeaponFirearmItemsSaveArray()
	{
		WeaponFirearmItems = TArray<FWeaponFirearmSaveData>();
	}
};
