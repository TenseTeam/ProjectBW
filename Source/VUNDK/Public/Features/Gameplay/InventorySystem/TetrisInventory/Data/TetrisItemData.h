// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "TetrisItemData.generated.h"

UCLASS(NotBlueprintType, Blueprintable)
class VUNDK_API UTetrisItemData : public UItemDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	FIntPoint Size;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeRotated;

public:
	UTetrisItemData()
	{
		ItemClass = UTetrisItem::StaticClass();
		Size = FIntPoint(1, 1);
		bCanBeRotated = false;
	}
};
