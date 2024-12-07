// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/SaveData/TetrisItemSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGItemVisualDetails.h"
#include "RPGItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FTetrisItemSaveData TetrisItemSaveData;
	UPROPERTY(BlueprintReadWrite)
	FName RarityID;
	UPROPERTY(BlueprintReadWrite)
	FRPGItemVisualDetails VisualDetails;
};
