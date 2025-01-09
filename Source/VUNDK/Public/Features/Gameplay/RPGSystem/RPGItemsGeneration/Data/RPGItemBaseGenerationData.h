// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Visuals/RPGItemVisualSetData.h"
#include "Engine/DataAsset.h"
#include "RPGItemBaseGenerationData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VUNDK_API URPGItemBaseGenerationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPGItemVisualSetData* ItemVisualDetailsSet;
};
