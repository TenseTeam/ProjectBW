// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BWWeaponsFactory.generated.h"

UCLASS()
class PROJECTBW_API UBWWeaponsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ProjectBW|WeaponSystem")
	static AWeaponBase* CreateWeapon(APawn* Owner, UBWWeaponItem* WeaponItem);

	static AWeaponBase* CreateWeaponBase(APawn* Owner, UBWWeaponItem* WeaponItem);
	
	UFUNCTION(BlueprintCallable, Category = "ProjectBW|WeaponSystem")
	static ABWWeaponFirearm* CreateWeaponFirearm(APawn* Owner, UBWWeaponFirearmItem* WeaponItem);

private:
	static AActor* SpawnWeaponActor(const UObject* Instigator, const UBWWeaponItem* WeaponItem);
};
