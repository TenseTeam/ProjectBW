// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/BaseStatData.h"
#include "ItemsStatsData.generated.h"

UCLASS()
class VUNDK_API UItemsStatsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UBaseStatData*> Stats;
};
