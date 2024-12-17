// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemBaseGenerationData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/BaseStatData.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "RPGGearItemGenerationData.generated.h"

UCLASS()
class VUNDK_API URPGGearItemGenerationData : public URPGItemBaseGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<UBaseStatData*, TSubclassOf<UStatOperation>> StatsModifiers;
};
