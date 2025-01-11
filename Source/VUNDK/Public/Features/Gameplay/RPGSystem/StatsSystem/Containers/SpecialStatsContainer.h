// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsContainer.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SpecialStatData.h"
#include "UObject/Object.h"
#include "SpecialStatsContainer.generated.h"

UCLASS(BlueprintType)
class VUNDK_API USpecialStatsContainer : public UStatsContainer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddSpecialStats(TSet<USpecialStatData*> SpecialStats);

	UFUNCTION(BlueprintPure)
	TMap<USpecialStatData*, float> GetSpecialStatsValues();
};
