// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGGearItemSaveData.h"
#include "RPGGearItemsSaveArray.generated.h"

USTRUCT(BlueprintType)
struct FRPGGearItemsSaveArray
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRPGGearItemSaveData> GearItems;

	FRPGGearItemsSaveArray()
	{
		GearItems = TArray<FRPGGearItemSaveData>();
	}
};
