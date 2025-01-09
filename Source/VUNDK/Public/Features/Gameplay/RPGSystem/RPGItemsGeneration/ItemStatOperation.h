// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "ItemStatOperation.generated.h"

UCLASS()
class VUNDK_API UItemStatOperation : public UStatOperation
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	URPGGearItem* RelatedItem;
	UPROPERTY(BlueprintReadOnly)
	int32 GearBaseValue;
	UPROPERTY(BlueprintReadOnly)
	int32 RarityValue;

protected:
	virtual void OnInit_Implementation() override;
};
