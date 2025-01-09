// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsBridgeBase.h"
#include "Features/Gameplay/EquipmentSystem/Equipment.h"
#include "CharacterStats.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UCharacterStats : public UStatsBridgeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UEquipment* Equipment;
	
public:
	UCharacterStats();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment);

protected:
	virtual void OnCalculateFullStatsValues_Implementation() override;

private:
	bool Check() const;
};
