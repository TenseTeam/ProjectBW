// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "WeaponFirearmItem.generated.h"

UCLASS()
class PROJECTBW_API UWeaponFirearmItem : public UWeaponItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponFireRate();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetWeaponMagSize();

	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponRange();
};
