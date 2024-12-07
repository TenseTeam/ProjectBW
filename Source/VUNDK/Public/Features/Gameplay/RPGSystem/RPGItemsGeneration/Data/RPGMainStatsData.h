// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Data/CharacterBaseStatData.h"
#include "RPGMainStatsData.generated.h"

UCLASS()
class VUNDK_API URPGMainStatsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCharacterBaseStatData*> MaintStatPool;
};
