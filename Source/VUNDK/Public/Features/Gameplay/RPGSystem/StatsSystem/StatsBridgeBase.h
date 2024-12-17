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

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UStatsBridgeBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	USpecialStatsContainer* SpecialStats;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UBaseStatsContainer* BaseStats;
	
public:
	UStatsBridgeBase();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;

	UFUNCTION(BlueprintCallable)
	void CalculateBaseStatAffectedBySpecialStat(TSubclassOf<UStatOperation> OperationClass, USpecialStatData* SpecialStatData, UBaseStatData* BaseStatData);
};
