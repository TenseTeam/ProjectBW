// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SpecialStatData.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "UObject/Object.h"
#include "SpecialStatsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnSpecialStatDefaultValueChanged
	);

UCLASS(BlueprintType)
class VUNDK_API USpecialStatsContainer : public UObject, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSpecialStatDefaultValueChanged OnSpecialStatDefaultValueChanged;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<USpecialStatData*> SpecialStats;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<USpecialStatData*, int32> DefaultValues;

public:
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;
	
	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintPure)
	int32 GetStatDefaultValue(const USpecialStatData* SpecialStat) const;
	
	UFUNCTION(BlueprintCallable)
	int32 SetStatDefaultValue(USpecialStatData* SpecialStat, const int32 Value);
	
	UFUNCTION(BlueprintCallable)
	int32 ModifyStatDefaultValue(USpecialStatData* SpecialStat, const int32 SumValue);

	UFUNCTION(BlueprintPure)
	TMap<USpecialStatData*, int32> const& GetDefaultValues() const;

	UFUNCTION(BlueprintPure)
	USpecialStatData* GetSpecialStatByCodeName(const FName& SpecialStatCodeName) const;

	UFUNCTION(BlueprintPure)
	USpecialStatData* GetSpecialStatByID(const FName& SpecialStatID) const;
};
