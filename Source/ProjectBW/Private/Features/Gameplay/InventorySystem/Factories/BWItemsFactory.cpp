// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Factories/BWItemsFactory.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

UBWWeaponItem* UBWItemsFactory::CreateWeaponItem(UWeaponItemData* Data)
{
	UBWWeaponItem* WeaponItem = Cast<UBWWeaponItem>(UISFactory::CreateItem(Data));

	if (!IsValid(WeaponItem))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to create Weapon Item."));
		return nullptr;
	}

	return WeaponItem;
}

UBWWeaponFirearmItem* UBWItemsFactory::CreateWeaponFirearmItem(UWeaponItemData* Data)
{
	UBWWeaponFirearmItem* WeaponFirearmItem = Cast<UBWWeaponFirearmItem>(CreateWeaponItem(Data));

	if (!IsValid(WeaponFirearmItem))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to create Weapon Firearm Item."));
		return nullptr;
	}

	return WeaponFirearmItem;
}
