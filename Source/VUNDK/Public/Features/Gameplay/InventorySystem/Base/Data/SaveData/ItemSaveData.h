// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EquipSlotIndex;

	FItemSaveData()
	{
		EquipSlotIndex = -1;
	}
};
