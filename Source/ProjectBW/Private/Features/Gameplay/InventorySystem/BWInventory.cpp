// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/BWInventory.h"

#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/BWInventorySaveData.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/WeaponItemsSaveArray.h"
#include "Features/Gameplay/InventorySystem/Factories/BWItemsFactory.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

UBWInventory::UBWInventory()
{
}

USaveData* UBWInventory::CreateSaveDataObject_Implementation()
{
	return NewObject<UBWInventorySaveData>();
}

USaveData* UBWInventory::CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave)
{
	UBWInventorySaveData* InventorySaveData = Cast<UBWInventorySaveData>(SaveData);
	CreateBWInventorySaveData(InventorySaveData->BWItemsSaveData, ItemsToSave);
	CreateRPGInventorySaveData(InventorySaveData->BWItemsSaveData.RPGItemsSaveData, ItemsToSave);
	return InventorySaveData;
}

void UBWInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	UBWInventorySaveData* BWInventorySaveData = Cast<UBWInventorySaveData>(InventorySaveData);
	LoadRPGInventorySaveData(BWInventorySaveData->BWItemsSaveData.RPGItemsSaveData);
	LoadBWInventorySaveData(BWInventorySaveData->BWItemsSaveData);
}

void UBWInventory::CreateBWInventorySaveData(FBWInventoryItemsSaveData& BWItemsSaveData, TArray<UItemBase*>& ItemsToSave)
{
	TArray<UItemBase*> RemaingItemsToSave = ItemsToSave;
	for (UItemBase* Item : ItemsToSave)
	{
		RemaingItemsToSave.Remove(Item);
		
		if (Item->IsA(UBWWeaponFirearmItem::StaticClass()))
		{
			const UBWWeaponFirearmItem* WeaponItem = Cast<UBWWeaponFirearmItem>(Item);
			FGuid ItemID = Item->GetItemData()->ItemDataID;

			if (!BWItemsSaveData.WeaponFirearmItems.Contains(ItemID))
				BWItemsSaveData.WeaponFirearmItems.Add(ItemID, FWeaponFirearmItemsSaveArray());

			FWeaponFirearmSaveData WeaponItemSaveData = WeaponItem->CreateWeaponFirearmSaveData();
			BWItemsSaveData.WeaponFirearmItems[ItemID].WeaponFirearmItems.Add(WeaponItemSaveData);
			continue;
		}

		if (Item->IsA(UBWWeaponItem::StaticClass()))
		{
			const UBWWeaponItem* WeaponItem = Cast<UBWWeaponItem>(Item);
			FGuid ItemID = Item->GetItemData()->ItemDataID;

			if (!BWItemsSaveData.GenericWeaponItems.Contains(ItemID))
				BWItemsSaveData.GenericWeaponItems.Add(ItemID, FWeaponItemsSaveArray());

			FWeaponItemSaveData WeaponItemSaveData = WeaponItem->CreateWeaponItemSaveData();
			BWItemsSaveData.GenericWeaponItems[ItemID].WeaponItems.Add(WeaponItemSaveData);
		}
	}

	ItemsToSave = RemaingItemsToSave;
}

void UBWInventory::LoadBWInventorySaveData(FBWInventoryItemsSaveData& BWItemsSaveData)
{
	for (auto& WeaponFirearmItems : BWItemsSaveData.WeaponFirearmItems)
	{
		const FGuid ItemID = WeaponFirearmItems.Key;

		for (FWeaponFirearmSaveData& WeaponFirearmSaveData : WeaponFirearmItems.Value.WeaponFirearmItems)
		{
			UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}

			if (UWeaponItemData* WeaponFirearmItemData = Cast<UWeaponItemData>(ItemData))
			{
				UBWWeaponFirearmItem* WeaponFirearmItem = Cast<UBWWeaponFirearmItem>(UBWItemsFactory::CreateWeaponItem(WeaponFirearmItemData));
				WeaponFirearmItem->LoadWeaponFirearmItemSaveData(this, WeaponFirearmSaveData);
			}
		}
	}
	
	for (auto& WeaponItems : BWItemsSaveData.GenericWeaponItems)
	{
		const FGuid ItemID = WeaponItems.Key;

		for (FWeaponItemSaveData& WeaponItemSaveData : WeaponItems.Value.WeaponItems)
		{
			UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}

			if (UWeaponItemData* WeaponItemData = Cast<UWeaponItemData>(ItemData))
			{
				UBWWeaponItem* WeaponItem = UBWItemsFactory::CreateWeaponItem(WeaponItemData);
				WeaponItem->LoadWeaponItemSaveData(this, WeaponItemSaveData);
			}
		}
	}
}
