// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItemSaveData.h"
#include "WeaponItemsSaveArray.generated.h"

USTRUCT(BlueprintType)
struct FWeaponItemsSaveArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponItemSaveData> WeaponItems;

	FWeaponItemsSaveArray()
	{
		WeaponItems = TArray<FWeaponItemSaveData>();
	}
};
