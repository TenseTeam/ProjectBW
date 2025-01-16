// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Items/WeaponItem.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"

UWeaponItemData* UWeaponItem::GetWeaponItemData() const
{
	return Cast<UWeaponItemData>(ItemData);
}

float UWeaponItem::GetWeaponDamage_Implementation()
{
	return GetWeaponItemData()->WeaponClass->GetDefaultObject<AWeaponBase>()->GetWeaponData().Damage;
}
