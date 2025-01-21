// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "RPGItemsSaveArray.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemsSaveArray
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRPGItemSaveData> Items;

	FRPGItemsSaveArray()
	{
		Items = TArray<FRPGItemSaveData>();
	}
};
