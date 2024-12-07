// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISFactory.generated.h"

UCLASS()
class VUNDK_API UISFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|Inventory System")
	static UItemBase* CreateItem(UItemDataBase* Data);
};
