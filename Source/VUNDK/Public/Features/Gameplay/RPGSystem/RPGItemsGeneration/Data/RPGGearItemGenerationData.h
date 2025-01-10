// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemBaseGenerationData.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/CoreStatData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/ItemStatOperation.h"
#include "RPGGearItemGenerationData.generated.h"

UCLASS()
class VUNDK_API URPGGearItemGenerationData : public URPGItemBaseGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<UCoreStatData*, TSubclassOf<UItemStatOperation>> StatsModifiers;
};
