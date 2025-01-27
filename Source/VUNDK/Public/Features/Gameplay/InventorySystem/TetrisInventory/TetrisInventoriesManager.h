// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisInventory.h"
#include "Features/Gameplay/InventorySystem/Base/InventoriesManager.h"
#include "TetrisInventoriesManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UTetrisInventoriesManager : public UInventoriesManager
{
	GENERATED_BODY()

public:
	UTetrisInventoriesManager();
	
	UFUNCTION(BlueprintCallable)
	bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* SourceInventory, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);
};
