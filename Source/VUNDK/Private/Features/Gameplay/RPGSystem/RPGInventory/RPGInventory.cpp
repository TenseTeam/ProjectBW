// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGWeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventorySaveData.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGISFactory.h"

URPGInventory::URPGInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}

USaveData* URPGInventory::CreateSaveData()
{
	URPGInventorySaveData* SaveData = NewObject<URPGInventorySaveData>();

	for (UItemBase* Item : GetAllItems())
	{
		if (Item->IsA(URPGGearItem::StaticClass()))
		{
			const URPGGearItem* GearItem = Cast<URPGGearItem>(Item);
			FName ItemID = Item->GetItemData()->GetItemDataID();

			if (!SaveData->ItemsSaveData.GearItems.Contains(ItemID))
				SaveData->ItemsSaveData.GearItems.Add(ItemID, FRPGGearItemsSaveArray());

			FRPGGearItemSaveData GearItemSaveData = GearItem->CreateRPGGearItemSaveData();
			SaveData->ItemsSaveData.GearItems[ItemID].GearItems.Add(GearItemSaveData);
			continue;
		}

		if (Item->IsA(URPGWeaponItem::StaticClass()))
		{
			const URPGWeaponItem* WeaponItem = Cast<URPGWeaponItem>(Item);
			FName ItemID = Item->GetItemData()->GetItemDataID();
			
			if (!SaveData->ItemsSaveData.WeaponItems.Contains(ItemID))
				SaveData->ItemsSaveData.WeaponItems.Add(ItemID, FRPGWeaponItemsSaveArray());

			FRPGItemSaveData WeaponItemSaveData = WeaponItem->CreateRPGItemSaveData();
			SaveData->ItemsSaveData.WeaponItems[ItemID].WeaponItems.Add(WeaponItemSaveData);
		}
	}

	return SaveData;
}

void URPGInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	URPGInventorySaveData* RPGInventorySaveData = Cast<URPGInventorySaveData>(InventorySaveData);
	
	for (TPair<FName, FRPGGearItemsSaveArray>& GearItems : RPGInventorySaveData->ItemsSaveData.GearItems)
	{
		const FName ItemID = GearItems.Key;
		
		for (FRPGGearItemSaveData& GearItemSaveData : GearItems.Value.GearItems)
		{
			UItemDataBase* ItemData = GetItemDataByID(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}
			
			if (URPGGearItemData* GearData = Cast<URPGGearItemData>(ItemData))
			{
				URPGGearItem* GearItem = URPGISFactory::CreateRPGGearItem(GearData, nullptr, false);
				GearItem->LoadRPGGearItemSaveData(GearItemSaveData, this);
			}
		}
	}

	for (TPair<FName, FRPGWeaponItemsSaveArray>& WeaponItems : RPGInventorySaveData->ItemsSaveData.WeaponItems)
	{
		const FName ItemID = WeaponItems.Key;
		
		for (FRPGItemSaveData& RPGItemSaveData : WeaponItems.Value.WeaponItems)
		{
			UItemDataBase* ItemData = GetItemDataByID(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}
			
			if (URPGWeaponItemData* WeaponData = Cast<URPGWeaponItemData>(ItemData))
			{
				URPGWeaponItem* WeaponItem = URPGISFactory::CreateRPGWeaponItem(WeaponData, nullptr, false);
				WeaponItem->LoadRPGWeaponItemSaveData(RPGItemSaveData, this);
			}
		}
	}
}
