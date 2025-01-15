// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Generic/SaveSystem/Data/SaveData.h"
#include "StatsBridgeSaveData.generated.h"

UCLASS()
class VUNDK_API UStatsBridgeSaveData : public USaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FGuid, float> SavedSpecialStats;
	UPROPERTY(BlueprintReadWrite)
	TMap<FGuid, float> SavedCoreStats;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Init(const TMap<FGuid, float> InSavedSpecialStats, const TMap<FGuid, float> InSavedCoreStats)
	{
		SavedSpecialStats = InSavedSpecialStats;
		SavedCoreStats = InSavedCoreStats;
	}
};
