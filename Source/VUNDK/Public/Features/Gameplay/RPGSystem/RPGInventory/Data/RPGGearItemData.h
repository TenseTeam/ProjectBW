// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemDataBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/StatOperation.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Data/CharacterBaseStatData.h"
#include "RPGGearItemData.generated.h"

UCLASS()
class VUNDK_API URPGGearItemData : public URPGItemDataBase
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
