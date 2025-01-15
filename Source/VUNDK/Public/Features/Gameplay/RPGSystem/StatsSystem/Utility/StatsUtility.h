// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatsUtility.generated.h"

UCLASS()
class VUNDK_API UStatsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "VUNDK|RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerPawnStats(int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category = "VUNDK|RPGSystem|Stats")
	static UStatsBridgeBase* GetPlayerControllerStats(const int32 PlayerIndex);
};
