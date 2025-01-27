// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"

FWeaponFirearmSaveData UBWWeaponFirearmItem::CreateWeaponFirearmSaveData() const
{
	FWeaponFirearmSaveData WeaponFirearmSaveData;
	WeaponFirearmSaveData.WeaponItemSaveData = CreateWeaponItemSaveData();
	WeaponFirearmSaveData.AmmoRemaining = AmmoRemaining;
	WeaponFirearmSaveData.bHasNewMag = bHasNewMag;
	return WeaponFirearmSaveData;
}

void UBWWeaponFirearmItem::LoadWeaponFirearmItemSaveData(UBWInventory* LoadingInventory, FWeaponFirearmSaveData& WeaponItemSaveData)
{
	AmmoRemaining = WeaponItemSaveData.AmmoRemaining;
	bHasNewMag = WeaponItemSaveData.bHasNewMag;
	LoadWeaponItemSaveData(LoadingInventory, WeaponItemSaveData.WeaponItemSaveData);
}

bool UBWWeaponFirearmItem::TryConsumeWeaponItemNewMagazine(const int MagSize)
{
	if (!bHasNewMag)
		return false;

	AmmoRemaining = MagSize;
	bHasNewMag = false;
	return true;
}

void UBWWeaponFirearmItem::SetAmmoRemaining(const int32 NewAmmoRemaining)
{
	AmmoRemaining = NewAmmoRemaining;
}

int32 UBWWeaponFirearmItem::GetAmmoRemaining() const
{
	return AmmoRemaining;
}

float UBWWeaponFirearmItem::GetFireRate_Implementation()
{
	return 0.0f;
}

float UBWWeaponFirearmItem::GetMaxRange_Implementation()
{
	return 0.0f;
}

int32 UBWWeaponFirearmItem::GetMagSize_Implementation()
{
	return 0;
}
