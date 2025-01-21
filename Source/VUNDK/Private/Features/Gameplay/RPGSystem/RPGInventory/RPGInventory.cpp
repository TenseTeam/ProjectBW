// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventorySaveData.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

URPGInventory::URPGInventory()
{
}

USaveData* URPGInventory::CreateSaveDataObject_Implementation()
{
	return NewObject<URPGInventorySaveData>();
}

USaveData* URPGInventory::CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave)
{
	URPGInventorySaveData* RPGInventorySaveData = Cast<URPGInventorySaveData>(SaveData);
	CreateRPGInventorySaveData(RPGInventorySaveData->RPGItemsSaveData, ItemsToSave);
	return SaveData;
}

void URPGInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	URPGInventorySaveData* RPGInventorySaveData = Cast<URPGInventorySaveData>(InventorySaveData);
	LoadRPGInventorySaveData(RPGInventorySaveData->RPGItemsSaveData);
}

void URPGInventory::CreateRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData, TArray<UItemBase*>& ItemsToSave)
{
	TArray<UItemBase*> RemaingToSave = ItemsToSave;
	for (UItemBase* Item : ItemsToSave)
	{
		RemaingToSave.Remove(Item);
		
		if (Item->IsA(URPGGearItem::StaticClass()))
		{
			const URPGGearItem* GearItem = Cast<URPGGearItem>(Item);
			FGuid ItemID = Item->GetItemData()->ItemDataID;

			if (!RPGItemsSaveData.GearItems.Contains(ItemID))
				RPGItemsSaveData.GearItems.Add(ItemID, FRPGGearItemsSaveArray());

			FRPGGearItemSaveData GearItemSaveData = GearItem->CreateRPGGearItemSaveData();
			RPGItemsSaveData.GearItems[ItemID].GearItems.Add(GearItemSaveData);
			continue;
		}

		// All items that are not gear items are considered generic items
		const URPGItem* RPGItem = Cast<URPGItem>(Item);
		FGuid ItemID = Item->GetItemData()->ItemDataID;

		if (!RPGItemsSaveData.GenericItems.Contains(ItemID))
			RPGItemsSaveData.GenericItems.Add(ItemID, FRPGItemsSaveArray());

		FRPGItemSaveData ItemSaveData = RPGItem->CreateRPGItemSaveData();
		RPGItemsSaveData.GenericItems[ItemID].Items.Add(ItemSaveData);
	}

	ItemsToSave = RemaingToSave;
}

void URPGInventory::LoadRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData)
{
	for (auto& GenericItems : RPGItemsSaveData.GenericItems)
	{
		const FGuid ItemID = GenericItems.Key;

		for (FRPGItemSaveData& RPGItemSaveData : GenericItems.Value.Items)
		{
			UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}

			if (URPGItemData* RPGItemData = Cast<URPGItemData>(ItemData))
			{
				URPGItem* RPGItem = URPGFactory::CreateRPGGenericItem(RPGItemData, nullptr, false);
				RPGItem->LoadRPGItemSaveData(this, RPGItemSaveData);
			}
		}
	}
	
	for (auto& GearItems : RPGItemsSaveData.GearItems)
	{
		const FGuid ItemID = GearItems.Key;

		for (FRPGGearItemSaveData& GearItemSaveData : GearItems.Value.GearItems)
		{
			UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID);

			if (ItemData == nullptr)
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Item data not found for item ID: %s"), *ItemID.ToString());
				continue;
			}

			if (URPGGearItemData* GearData = Cast<URPGGearItemData>(ItemData))
			{
				URPGGearItem* GearItem = URPGFactory::CreateRPGGearItem(GearData, nullptr, false);
				GearItem->LoadRPGGearItemSaveData(this, GearItemSaveData);
			}
		}
	}
}
