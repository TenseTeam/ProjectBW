// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpecialStatData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API USpecialStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName StatID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor StatColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* StatIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StatDefaultValue;

public:
	USpecialStatData(): StatID(FGuid::NewGuid().ToString()),
	                    StatCodeName("AAA"),
	                    StatIcon(nullptr),
	                    StatDefaultValue(10)
	{
	}
};
