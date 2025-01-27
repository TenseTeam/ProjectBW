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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCalculatedCoreStatsValues,
	UStatsBridgeBase*, StatsBridge
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnCalculatedFullStatsValues,
	UStatsBridgeBase*, StatsBridge
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
	
private:
	UPROPERTY()
	UCoreStatsContainer* FullStatsContainer;
	
public:
	UStatsBridgeBase();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;
	
	USpecialStatData* GetSpecialStatByID(const FGuid SpecialStatID) const;

	UCoreStatData* GetCoreStatByID(const FGuid CoreStatID) const;

	UFUNCTION(BlueprintPure)
	TMap<UCoreStatData*, float> GetFullStatsValues() const;

	UFUNCTION(BlueprintPure)
	float GetFullStatValueAsFloat(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	int32 GetFullStatValueAsInt(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	FString GetFullStatValueAsString(const UStatDataBase* Stat) const;

	UFUNCTION(BlueprintPure)
	int32 GetFullStatsLength() const;
	
	UFUNCTION(BlueprintCallable)
	void CalculateCoreStatValueWithSpecialStat(USpecialStatData* SpecialStatData, UCoreStatData* CoreStatData, const TSubclassOf<UStatOperation> OperationClass);

	UFUNCTION(BlueprintCallable)
	void CalculateAllStatsValues();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SetFullStatValue(UStatDataBase* Stat, float Value) const;

	UFUNCTION(BlueprintCallable)
	void ModifyFullStatValue(UStatDataBase* Stat, float Value) const;
	
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
