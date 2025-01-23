// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BWItemsFactory.generated.h"

UCLASS()
class PROJECTBW_API UBWItemsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ProjectBW|InventorySystem")
	static UBWWeaponItem* CreateWeaponItem(UWeaponItemData* Data);

	UFUNCTION(BlueprintCallable, Category = "ProjectBW|InventorySystem")
	static UBWWeaponFirearmItem* CreateWeaponFirearmItem(UWeaponItemData* Data);
};
