// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/InventoriesManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISInventoriesUtility.generated.h"

UCLASS()
class VUNDK_API UISInventoriesUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UInventoriesManager* InventoriesManager;

public:
	static void Init(UInventoriesManager* Manager);

	static bool Check();
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static UInventoriesManager* GetInventoriesManager();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static UItemsRegistry* GetRegistry();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static bool IsItemInRegistry(const UItemDataBase* ItemData);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Tetris Inventory System")
	static UItemDataBase* GetItemDataFromRegistry(const FName& ItemDataID);
};
