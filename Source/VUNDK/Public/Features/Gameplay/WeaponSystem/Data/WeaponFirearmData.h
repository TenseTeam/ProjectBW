// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmmoTypeData.h"
#include "Engine/DataAsset.h"
#include "WeaponFirearmData.generated.h"

USTRUCT(BlueprintType)
struct VUNDK_API FWeaponFirearmData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAmmoTypeData* AmmoType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"))
	float ADSPrecisionIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "100.0", UIMax = "100.0"))
	float ADSRecoilControlIncrease;

	FWeaponFirearmData(): AmmoType(nullptr),
	                      ADSPrecisionIncrease(0.0f),
	                      ADSRecoilControlIncrease(0.0f)
	{
	}
};
