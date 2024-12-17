// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseStatData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UBaseStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName StatID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MaxLength = 3))
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MeasurementSymbol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUncapped;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUncapped", EditConditionHides))
	int32 StatMinValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUncapped", EditConditionHides))
	int32 StatMaxValue;

public:
	UBaseStatData(): StatID(FGuid::NewGuid().ToString()),
	                 StatCodeName("AAA"),
	                 bIsUncapped(false),
	                 StatMinValue(0),
	                 StatMaxValue(1)
	{
	}
};
