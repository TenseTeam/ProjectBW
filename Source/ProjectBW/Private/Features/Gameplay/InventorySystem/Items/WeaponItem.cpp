// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Items/WeaponItem.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"

FWeaponItemSaveData UWeaponItem::CreateWeaponItemSaveData() const
{
	FWeaponItemSaveData WeaponItemSaveData;
	WeaponItemSaveData.GearItemSaveData = CreateRPGGearItemSaveData();
	return WeaponItemSaveData;
}

void UWeaponItem::LoadWeaponItemSaveData(UBWInventory* LoadingInventory, FWeaponItemSaveData& WeaponItemSaveData)
{
	LoadRPGGearItemSaveData(LoadingInventory, WeaponItemSaveData.GearItemSaveData);
}

UWeaponItemData* UWeaponItem::GetWeaponItemData() const
{
	return Cast<UWeaponItemData>(ItemData);
}

float UWeaponItem::GetWeaponDamage_Implementation()
{
	return GetWeaponItemData()->WeaponClass->GetDefaultObject<AWeaponBase>()->GetWeaponData().Damage;
}
