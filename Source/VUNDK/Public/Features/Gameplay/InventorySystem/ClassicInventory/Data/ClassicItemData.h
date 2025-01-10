// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/ClassicItem.h"
#include "UObject/Object.h"
#include "ClassicItemData.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class VUNDK_API UClassicItemData : public UItemDataBase
{
	GENERATED_BODY()

public:
	UClassicItemData()
	{
		ItemClass = UClassicItem::StaticClass();
	}
};
