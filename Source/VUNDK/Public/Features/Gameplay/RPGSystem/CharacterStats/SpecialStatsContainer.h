// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterSpecialStatData.h"
#include "UObject/Object.h"
#include "SpecialStatsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnSpecialStatDefaultValueChanged
	);

UCLASS(BlueprintType)
class VUNDK_API USpecialStatsContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSpecialStatDefaultValueChanged OnSpecialStatDefaultValueChanged;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCharacterSpecialStatData*> SpecialStats;
	
protected:
	UPROPERTY()
	TMap<UCharacterSpecialStatData*, int32> DefaultValues;

public:
	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintPure)
	int32 GetStatDefaultValue(const UCharacterSpecialStatData* SpecialStat) const;
	
	UFUNCTION(BlueprintCallable)
	int32 SetStatDefaultValue(UCharacterSpecialStatData* SpecialStat, const int32 Value);
	
	UFUNCTION(BlueprintCallable)
	int32 AddStatDefaultValue(UCharacterSpecialStatData* SpecialStat, const int32 Value);

	TMap<UCharacterSpecialStatData*, int32> const& GetDefaultValues() const;
	
	UCharacterSpecialStatData* GetSpecialStatByCodeName(const FName& SpecialStatCodeName) const;
};
