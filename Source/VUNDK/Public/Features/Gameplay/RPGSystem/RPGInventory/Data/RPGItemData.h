// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/TetrisItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"
#include "RPGItemData.generated.h"

UCLASS()
class VUNDK_API URPGItemData : public UTetrisItemData
{
	GENERATED_BODY()

public:
	URPGItemData()
	{
		ItemClass = URPGItem::StaticClass();
	}
};
