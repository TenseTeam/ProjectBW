// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemBaseGenerationData.h"
#include "RPGMainStatsData.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/StatOperation.h"
#include "RPGGearItemGenerationData.generated.h"

UCLASS()
class VUNDK_API URPGGearItemGenerationData : public URPGItemBaseGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPGMainStatsData* MainStatsPool;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<UCharacterBaseStatData*, TSubclassOf<UStatOperation>> StatsModifiers;
};
