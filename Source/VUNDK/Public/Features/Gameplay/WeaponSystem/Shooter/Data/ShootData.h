// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootType.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EShootType ShootType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", ClampMax = "360", UIMax = "360"))
	float MaxSpreadDegree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MaxRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	int32 MagSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasRecoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float RecoilStrength = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides, ClampMin = "0", UIMin = "0"))
	float RecoilDuration = .1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bHasRecoil", EditConditionHides,
		ToolTip = "The Y and Z axes define recoil strength per shot (horizontal = shots, vertical = strength), while the X axis controls recoil decay over time."))
	UCurveVector* RecoilCurve;

	FShootData(): Damage(0),
	              FireRate(0),
	              ShootType(),
	              MaxSpreadDegree(0),
	              MaxRange(0),
	              MagSize(0),
	              bHasRecoil(false),
	              RecoilCurve(nullptr)
	{
	}
};
