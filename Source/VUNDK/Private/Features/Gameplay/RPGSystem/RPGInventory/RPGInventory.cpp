// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGInventorySaveData.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

URPGInventory::URPGInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}

USaveData* URPGInventory::CreateSaveData()
{
	URPGInventorySaveData* SaveData = NewObject<URPGInventorySaveData>();

	for (UItemBase* Item : GetItems())
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

		const URPGItem* RPGItem = Cast<URPGItem>(Item);
		FName ItemID = Item->GetItemData()->GetItemDataID();

		if (!SaveData->ItemsSaveData.GenericItems.Contains(ItemID))
			SaveData->ItemsSaveData.GenericItems.Add(ItemID, FRPGItemsSaveArray());

		FRPGItemSaveData ItemSaveData = RPGItem->CreateRPGItemSaveData();
		SaveData->ItemsSaveData.GenericItems[ItemID].Items.Add(ItemSaveData);
	}

	return SaveData;
}

void URPGInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	URPGInventorySaveData* RPGInventorySaveData = Cast<URPGInventorySaveData>(InventorySaveData);

	for (TPair<FName, FRPGItemsSaveArray>& GenericItems : RPGInventorySaveData->ItemsSaveData.GenericItems)
	{
		const FName ItemID = GenericItems.Key;

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
				URPGItem* RPGItem = URPGFactory::CreateRPGItem(RPGItemData, nullptr, false);
				RPGItem->LoadRPGItemSaveData(RPGItemSaveData, this);
				AddLoadedTetrisItem(RPGItemData, RPGItemSaveData.TetrisItemSaveData, RPGItem);
			}
		}
	}
	
	for (TPair<FName, FRPGGearItemsSaveArray>& GearItems : RPGInventorySaveData->ItemsSaveData.GearItems)
	{
		const FName ItemID = GearItems.Key;

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
				GearItem->LoadRPGGearItemSaveData(GearItemSaveData, this);
				AddLoadedTetrisItem(GearData, GearItemSaveData.RPGItemSaveData.TetrisItemSaveData, GearItem);
			}
		}
	}
}
