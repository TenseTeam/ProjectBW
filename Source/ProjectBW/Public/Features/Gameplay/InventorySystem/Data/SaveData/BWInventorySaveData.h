// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BWInventoryItemsSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventorySaveData.h"
#include "BWInventorySaveData.generated.h"

UCLASS()
class PROJECTBW_API UBWInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FBWInventoryItemsSaveData BWItemsSaveData;
};
