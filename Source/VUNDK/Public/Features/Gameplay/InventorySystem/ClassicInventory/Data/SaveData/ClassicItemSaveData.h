// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/ItemSaveData.h"
#include "ClassicItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct FClassicItemSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSaveData ItemSaveData;
};
