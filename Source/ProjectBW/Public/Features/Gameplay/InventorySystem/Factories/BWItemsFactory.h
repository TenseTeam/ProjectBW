// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/WeaponItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BWItemsFactory.generated.h"

UCLASS()
class PROJECTBW_API UBWItemsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ProjectBW|InventorySystem")
	static UWeaponItem* CreateWeaponItem(UWeaponItemData* Data);
};
