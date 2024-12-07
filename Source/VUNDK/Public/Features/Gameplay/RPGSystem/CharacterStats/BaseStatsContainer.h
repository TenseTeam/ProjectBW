// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterBaseStatData.h"
#include "UObject/Object.h"
#include "BaseStatsContainer.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UBaseStatsContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCharacterBaseStatData*> BaseStats;
	
protected:
	UPROPERTY()
	TMap<UCharacterBaseStatData*, int32> DefaultValues;

public:
	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintPure)
	int32 GetStatDefaultValue(const UCharacterBaseStatData* BaseStat) const;
	
	UFUNCTION(BlueprintCallable)
	int32 SetStatDefaultValue(UCharacterBaseStatData* BaseStat, const int32 Value);
	
	UFUNCTION(BlueprintCallable)
	int32 AddStatDefaultValue(const UCharacterBaseStatData* BaseStat, const int32 Value);

	TMap<UCharacterBaseStatData*, int32> const& GetDefaultValues() const;
	
	UCharacterBaseStatData* GetBaseStatByCodeName(const FName& BaseStatCodeName) const;
};
