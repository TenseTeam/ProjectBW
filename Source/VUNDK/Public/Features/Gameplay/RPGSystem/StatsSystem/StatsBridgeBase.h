// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatOperation.h"
#include "Components/ActorComponent.h"
#include "Containers/CoreStatsContainer.h"
#include "Containers/SpecialStatsContainer.h"
#include "Data/SpecialStatData.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "StatsBridgeBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogStatsSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCalculatedCoreStatsValues
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCalculatedFullStatsValues,
	UCoreStatsContainer*, FullStatsContainer
);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UStatsBridgeBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCalculatedCoreStatsValues OnCalculatedCoreStatsValues;
	UPROPERTY(BlueprintAssignable)
	FOnCalculatedFullStatsValues OnCalculatedFullStatsValues;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<USpecialStatData*> SpecialStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<UCoreStatData*> CoreStats;
	UPROPERTY(BlueprintReadOnly)
	USpecialStatsContainer* SpecialStatsContainer;
	UPROPERTY(BlueprintReadOnly)
	UCoreStatsContainer* CoreStatsContainer;
	UPROPERTY(BlueprintReadOnly)
	UCoreStatsContainer* FullStatsContainer;
	
public:
	UStatsBridgeBase();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;
	
	UCoreStatData* GetCoreStatByID(const FName& BaseStatID) const;
	
	USpecialStatData* GetSpecialStatByID(const FName& SpecialStatID) const;

	UFUNCTION(BlueprintCallable)
	void CalculateCoreStatValueWithSpecialStat(USpecialStatData* SpecialStatData, UCoreStatData* CoreStatData, const TSubclassOf<UStatOperation> OperationClass);

	UFUNCTION(BlueprintCallable)
	void CalculateAllStatsValues();
	
protected:
	UFUNCTION()
	void CalculateCoreStatsValues();

	UFUNCTION()
	void CalculateFullStatsValues();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateCoreStatsValues();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCalculateFullStatsValues();

private:
	void CreateStatsContainers();
};
