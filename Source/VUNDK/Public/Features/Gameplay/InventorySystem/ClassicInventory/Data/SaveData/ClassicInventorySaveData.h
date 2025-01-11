// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/InventoryBaseSaveData.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/Data/SaveData/ClassicItemSaveData.h"
#include "ClassicInventorySaveData.generated.h"

UCLASS()
class VUNDK_API UClassicInventorySaveData : public UInventoryBaseSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGuid, FClassicItemSaveData> SavedItems;
};
