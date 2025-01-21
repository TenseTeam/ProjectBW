// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Items/WeaponFirearmItem.h"

FWeaponFirearmSaveData UWeaponFirearmItem::CreateWeaponFirearmSaveData() const
{
	FWeaponFirearmSaveData WeaponFirearmSaveData;
	WeaponFirearmSaveData.WeaponItemSaveData = CreateWeaponItemSaveData();
	WeaponFirearmSaveData.AmmoRemaining = AmmoRemaining;
	return WeaponFirearmSaveData;
}

void UWeaponFirearmItem::LoadWeaponFirearmItemSaveData(UBWInventory* LoadingInventory, FWeaponFirearmSaveData& WeaponItemSaveData)
{
	AmmoRemaining = WeaponItemSaveData.AmmoRemaining;
	LoadWeaponItemSaveData(LoadingInventory, WeaponItemSaveData.WeaponItemSaveData);
}

void UWeaponFirearmItem::SetAmmoRemaining(const int32 NewAmmoRemaining)
{
	AmmoRemaining = NewAmmoRemaining;
}

int32 UWeaponFirearmItem::GetAmmoRemaining() const
{
	return AmmoRemaining;
}

float UWeaponFirearmItem::GetWeaponFireRate_Implementation()
{
	return 0.0f;
}

int32 UWeaponFirearmItem::GetWeaponMagSize_Implementation()
{
	return 0;
}

float UWeaponFirearmItem::GetWeaponRange_Implementation()
{
	return 0.0f;
}
