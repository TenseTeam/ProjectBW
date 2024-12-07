// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/TetrisItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItemBase.h"
#include "RPGItemDataBase.generated.h"

UCLASS(Abstract)
class VUNDK_API URPGItemDataBase : public UTetrisItemData
{
	GENERATED_BODY()

public:
	URPGItemDataBase()
	{
		ItemClass = URPGItemBase::StaticClass();
	}
};
