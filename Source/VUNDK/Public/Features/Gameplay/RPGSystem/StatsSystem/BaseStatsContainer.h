// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/BaseStatData.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "UObject/Object.h"
#include "BaseStatsContainer.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UBaseStatsContainer : public UObject, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UBaseStatData*> BaseStats;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<UBaseStatData*, int32> BaseValues;

public:
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;
	
	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintPure)
	int32 GetStatBaseValue(const UBaseStatData* BaseStat) const;
	
	UFUNCTION(BlueprintCallable)
	int32 SetStatBaseValue(UBaseStatData* BaseStat, const int32 Value);
	
	UFUNCTION(BlueprintCallable)
	int32 ModifyStatBaseValue(UBaseStatData* BaseStat, const int32 SumValue);

	UFUNCTION(BlueprintPure)
	UBaseStatData* GetBaseStatByCodeName(const FName& BaseStatCodeName) const;

	UFUNCTION(BlueprintPure)
	UBaseStatData* GetBaseStatByID(const FName& BaseStatID) const;

	UFUNCTION(BlueprintPure)
	TMap<UBaseStatData*, int32> const& GetBaseValues() const;
};
