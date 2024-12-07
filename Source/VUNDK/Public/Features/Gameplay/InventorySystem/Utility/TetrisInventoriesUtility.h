// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventoriesManager.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TetrisInventoriesUtility.generated.h"

UCLASS()
class VUNDK_API UTetrisInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static UTetrisInventoriesManager* GetTetrisInventoriesManager();
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static bool TryTransferItem(UTetrisItem* Item, UTetrisInventory* SourceInventory, UTetrisInventory* DestinationInventory, const FIntPoint& DestinationPosition);
};
