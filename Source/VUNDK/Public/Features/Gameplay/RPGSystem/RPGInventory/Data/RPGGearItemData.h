// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/Data/CoreStatData.h"
#include "RPGGearItemData.generated.h"

UCLASS()
class VUNDK_API URPGGearItemData : public URPGItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GearBaseMultiplier = 1.0f;

public:
	URPGGearItemData()
	{
		ItemClass = URPGGearItem::StaticClass();
	}
};
