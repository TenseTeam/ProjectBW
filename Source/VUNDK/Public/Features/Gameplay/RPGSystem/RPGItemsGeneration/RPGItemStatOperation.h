// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "RPGItemStatOperation.generated.h"

UCLASS()
class VUNDK_API URPGItemStatOperation : public UStatOperation
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	URPGGearItem* RelatedItem;
	UPROPERTY(BlueprintReadOnly)
	float GearBaseValue;
	UPROPERTY(BlueprintReadOnly)
	float RarityValue;

protected:
	virtual void OnInit_Implementation() override;
};
