// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsSaveData.h"
#include "Features/Generic/SaveSystem/Data/SaveData.h"
#include "StatsBridgeSaveData.generated.h"

UCLASS()
class VUNDK_API UStatsBridgeSaveData : public USaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UStatsSaveData* BaseStatsSaveData;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	UStatsSaveData* SpecialStatsSaveData;
};
