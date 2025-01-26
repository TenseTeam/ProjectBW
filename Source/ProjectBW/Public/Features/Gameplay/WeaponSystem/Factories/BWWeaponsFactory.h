// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/WeaponSystem/BWWeaponMelee.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BWWeaponsFactory.generated.h"

UCLASS()
class PROJECTBW_API UBWWeaponsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AWeaponBase* CreateWeapon(APawn* Owner, UBWWeaponItem* WeaponItem);

	static ABWWeaponMelee* CreateBWWeaponMelee(APawn* Owner, UBWWeaponItem* WeaponItem);
	
	static ABWWeaponFirearm* CreateBWWeaponFirearm(APawn* Owner, UBWWeaponFirearmItem* WeaponItem);

private:
	static AActor* SpawnWeaponActor(const UObject* Instigator, const UBWWeaponItem* WeaponItem);
};
