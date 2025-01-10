// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGRarityLevelData.h"
#include "Engine/DataAsset.h"
#include "RPGItemsRaritiesData.generated.h"

UCLASS()
class VUNDK_API URPGItemsRaritiesData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<URPGRarityLevelData*> RarityLevels;
};
