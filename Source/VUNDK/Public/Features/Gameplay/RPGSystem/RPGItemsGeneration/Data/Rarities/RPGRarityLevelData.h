// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGRarityLevelData.generated.h"

UCLASS(BlueprintType, NotBlueprintable)
class VUNDK_API URPGRarityLevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid RarityID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RarityName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RarityColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float Probability;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RarityValue;

public:
	URPGRarityLevelData() : RarityID(FGuid::NewGuid()),
	                        RarityName(FText::GetEmpty()),
	                        RarityColor(FLinearColor::White),
	                        Probability(0.f),
	                        RarityValue(0.f)
	{
	}
};
