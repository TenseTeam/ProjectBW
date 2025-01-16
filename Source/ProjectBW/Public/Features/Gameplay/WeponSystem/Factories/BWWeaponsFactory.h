// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/WeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/WeaponItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BWWeaponsFactory.generated.h"

UCLASS()
class PROJECTBW_API UBWWeaponsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ProjectBW|WeaponSystem")
	static AWeaponBase* CreateWeapon(UObject* Instigator, UWeaponItem* WeaponItem);

	UFUNCTION(BlueprintCallable, Category = "ProjectBW|WeaponSystem")
	static AWeaponFirearm* CreateWeaponFirearm(UObject* Instigator, UWeaponFirearmItem* WeaponItem);

private:
	static AActor* SpawnWeaponActor(const UObject* Instigator, const UWeaponItem* WeaponItem);
};
