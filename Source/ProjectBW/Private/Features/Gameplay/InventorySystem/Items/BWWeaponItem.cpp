// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"

FWeaponItemSaveData UBWWeaponItem::CreateWeaponItemSaveData() const
{
	FWeaponItemSaveData WeaponItemSaveData;
	WeaponItemSaveData.GearItemSaveData = CreateRPGGearItemSaveData();
	return WeaponItemSaveData;
}

void UBWWeaponItem::LoadWeaponItemSaveData(UBWInventory* LoadingInventory, FWeaponItemSaveData& WeaponItemSaveData)
{
	LoadRPGGearItemSaveData(LoadingInventory, WeaponItemSaveData.GearItemSaveData);
}

UWeaponItemData* UBWWeaponItem::GetWeaponItemData() const
{
	return Cast<UWeaponItemData>(ItemData);
}

float UBWWeaponItem::GetWeaponDamage_Implementation()
{
	return GetWeaponItemData()->WeaponClass->GetDefaultObject<AWeaponBase>()->GetWeaponData().Damage;
}
