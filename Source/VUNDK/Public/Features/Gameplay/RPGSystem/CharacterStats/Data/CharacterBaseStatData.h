// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterBaseStatData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UCharacterBaseStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MeasurementSymbol;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StatMinValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StatMaxValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUncapped;
};
