// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsContainer.h"
#include "CoreStatsContainer.generated.h"

UCLASS()
class VUNDK_API UCoreStatsContainer : public UStatsContainer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddCoreStats(TSet<UCoreStatData*> CoreStats);

	UFUNCTION(BlueprintPure)
	TMap<UCoreStatData*, int32> GetCoreStatsValues();
};
