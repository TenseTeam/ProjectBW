// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveArrays/RPGGearItemsSaveArray.h"
#include "SaveArrays/RPGItemsSaveArray.h"
#include "RPGInventoryItemsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRPGInventoryItemsSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FRPGItemsSaveArray> GenericItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FRPGGearItemsSaveArray> GearItems;

	FRPGInventoryItemsSaveData()
	{
		GearItems = TMap<FName, FRPGGearItemsSaveArray>();
		GenericItems = TMap<FName, FRPGItemsSaveArray>();
	}
};
