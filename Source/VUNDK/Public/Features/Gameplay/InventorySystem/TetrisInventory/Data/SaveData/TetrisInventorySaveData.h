// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisItemsSaveArray.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/InventoryBaseSaveData.h"
#include "TetrisInventorySaveData.generated.h"

UCLASS()
class VUNDK_API UTetrisInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FTetrisItemsSaveArray> TetrisItems;
};
