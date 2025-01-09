// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/CoreStatData.h"
#include "UObject/Object.h"
#include "StatsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnStatsValuesChanged
);

UCLASS(BlueprintType)
class VUNDK_API UStatsContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnStatsValuesChanged OnStatsValuesChanged;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<UStatDataBase*, int32> Values;

public:
	UFUNCTION(BlueprintCallable)
	void AddStats(TSet<UStatDataBase*> Stats);
	
	UFUNCTION(BlueprintCallable)
	void AddStat(UStatDataBase* Stat, const int32 Value);

	UFUNCTION(BlueprintCallable)
	void ClearStats();

	UFUNCTION(BlueprintCallable)
	void RemoveStat(UStatDataBase* Stat);

	UFUNCTION(BlueprintCallable)
	void CopyStats(UStatsContainer* StatsContainer);

	UFUNCTION(BlueprintCallable)
	bool AreStatsEqual(UStatsContainer* StatsContainer) const;
	
	UFUNCTION(BlueprintPure)
	int32 GetValue(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintCallable)
	bool TrySetValue(UStatDataBase* Stat, const int32 Value);
	
	UFUNCTION(BlueprintCallable)
	bool TryModifyValue(UStatDataBase* Stat, const int32 SumValue);
	
	UFUNCTION(BlueprintPure)
	TMap<UStatDataBase*, int32> const& GetValues() const;
};
