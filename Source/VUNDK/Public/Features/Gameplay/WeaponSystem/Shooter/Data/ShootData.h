// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "Engine/DataAsset.h"
#include "ShootData.generated.h"

USTRUCT(BlueprintType)
struct VUNDK_API FShootData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", ToolTip = "rounds/min"))
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", ToolTip = "meters"))
	float Range;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	int32 MagSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float RecoilStrength = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float RecoilImpulseDuration = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides,
		ToolTip = "The Horizontal axis is the number of shots fired while the Vertical axis is the recoil strength."))
	UCurveVector* RecoilCurve;
};
