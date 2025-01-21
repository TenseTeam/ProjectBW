// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Factories/BWItemsFactory.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

UWeaponItem* UBWItemsFactory::CreateWeaponItem(UWeaponItemData* Data)
{
	UWeaponItem* WeaponItem = Cast<UWeaponItem>(UISFactory::CreateItem(Data));

	if (!IsValid(WeaponItem))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to create Weapon Item."));
		return nullptr;
	}

	return WeaponItem;
}
