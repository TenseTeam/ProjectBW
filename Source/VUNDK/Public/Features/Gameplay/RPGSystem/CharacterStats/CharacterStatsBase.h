// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseStatsContainer.h"
#include "SpecialStatsContainer.h"
#include "Components/ActorComponent.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "CharacterStatsBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UCharacterStatsBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	USpecialStatsContainer* SpecialStats;
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UBaseStatsContainer* BaseStats;
	
public:
	UCharacterStatsBase();

public:
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool LoadSaveData(USaveData* SavedData) override;
};
