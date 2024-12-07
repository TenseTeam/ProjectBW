// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGInventoryItemsSaveData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/SaveData/TetrisInventorySaveData.h"
#include "RPGInventorySaveData.generated.h"

UCLASS()
class VUNDK_API URPGInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FRPGInventoryItemsSaveData ItemsSaveData;
};
