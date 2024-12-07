// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGWeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"

void URPGWeaponItem::LoadRPGWeaponItemSaveData(const FRPGItemSaveData& RPGItemSaveData, URPGInventory* Inventory)
{
	LoadRPGItemSaveData(RPGItemSaveData, Inventory);
	WeaponDamageOperation = URPGItemsGenerator::CreateWeaponDamageOperation(this);
}

int32 URPGWeaponItem::GetRollWeaponDamage() const
{
	if (!WeaponDamageOperation)
		return 0;
	
	return WeaponDamageOperation->GetResultOperation();
}

URPGWeaponItemData* URPGWeaponItem::GetRPGWeaponData() const
{
	return Cast<URPGWeaponItemData>(ItemData);
}
