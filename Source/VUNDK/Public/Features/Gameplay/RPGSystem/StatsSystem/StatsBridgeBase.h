// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseStatsContainer.h"
#include "SpecialStatsContainer.h"
#include "StatOperation.h"
#include "Components/ActorComponent.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "StatsBridgeBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogStatsSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnBaseStatsValuesChanged
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnFullStatsValuesChanged
);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UStatsBridgeBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBaseStatsValuesChanged OnBaseStatsValuesChanged;
	UPROPERTY(BlueprintAssignable)
	FOnFullStatsValuesChanged OnFullStatsValuesChanged;
	
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	USpecialStatsContainer* SpecialStatsContainer;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UBaseStatsContainer* BaseStatsContainer;

protected:
	UPROPERTY()
	TMap<UBaseStatData*, int32> FullValues;
	
public:
	UStatsBridgeBase();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;

	UFUNCTION(BlueprintPure)
	TMap<UBaseStatData*, int32> const& GetFullStatsValues() const;
	
	UFUNCTION(BlueprintCallable)
	void SetStatBaseValueBySpecialStat(USpecialStatData* SpecialStatData, UBaseStatData* BaseStatData);

	UFUNCTION(BlueprintCallable)
	void CalculateAllStatsValues();
	
protected:
	UFUNCTION()
	void CalculateBaseStatsValues();

	UFUNCTION()
	void CalculateFullStatsValues();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateBaseStatsValues();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateFullStatsValues();
};
