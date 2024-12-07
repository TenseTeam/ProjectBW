// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/ItemSaveData.h"
#include "TetrisItemSaveData.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FTetrisItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSaveData ItemSaveData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint SlotPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotated;

	FTetrisItemSaveData()
	{
		ItemSaveData = FItemSaveData();
		SlotPosition = FIntPoint();
		bIsRotated = false;
	}
};
