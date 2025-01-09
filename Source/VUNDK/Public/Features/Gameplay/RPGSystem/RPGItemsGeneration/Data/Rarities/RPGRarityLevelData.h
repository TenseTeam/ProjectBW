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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName RarityID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText RarityName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor RarityColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float Probability;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RarityValue;

public:
	URPGRarityLevelData() : RarityID(FName("NotGeneratedID")),
	                        RarityName(FText::GetEmpty()),
	                        RarityColor(FLinearColor::White),
	                        Probability(0.0f),
	                        RarityValue(0)
	{
	}
	
	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		RarityID = FName(FGuid::NewGuid().ToString());
	}
};
