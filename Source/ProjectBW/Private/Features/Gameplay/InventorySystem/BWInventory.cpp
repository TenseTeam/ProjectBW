// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/BWInventory.h"

#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/BWInventorySaveData.h"
#include "Features/Gameplay/InventorySystem/Data/SaveData/WeaponItemsSaveArray.h"
#include "Features/Gameplay/InventorySystem/Factories/BWItemsFactory.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"

UBWInventory::UBWInventory(): AmmoInventory(nullptr)
{
}

void UBWInventory::BeginPlay()
{
	Super::BeginPlay();
	AmmoInventory->Init();
}

USaveData* UBWInventory::CreateSaveDataObject_Implementation()
{
	return NewObject<UBWInventorySaveData>();
}

USaveData* UBWInventory::CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave)
{
	UBWInventorySaveData* InvSaveData = Cast<UBWInventorySaveData>(SaveData);
	CreateBWInventorySaveData(InvSaveData->SaveDataWrapper, ItemsToSave);
	CreateRPGInventorySaveData(InvSaveData->SaveDataWrapper.RPGItemsSaveData, ItemsToSave);
	return InvSaveData;
}

void UBWInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	UBWInventorySaveData* InvSaveData = Cast<UBWInventorySaveData>(InventorySaveData);
	LoadRPGInventorySaveData(InvSaveData->SaveDataWrapper.RPGItemsSaveData);
	LoadBWInventorySaveData(InvSaveData->SaveDataWrapper);
}

void UBWInventory::CreateBWInventorySaveData(FBWInventorySaveDataWrapper& SaveDataWrapper, TArray<UItemBase*>& ItemsToSave) const
{
	TArray<UItemBase*> RemaingItemsToSave = ItemsToSave;
	for (UItemBase* Item : ItemsToSave)
	{
		if (Item->IsA(UBWWeaponFirearmItem::StaticClass()))
		{
			const UBWWeaponFirearmItem* WeaponItem = Cast<UBWWeaponFirearmItem>(Item);
			FGuid ItemID = Item->GetItemData()->ItemDataID;

			if (!SaveDataWrapper.WeaponFirearmItems.Contains(ItemID))
				SaveDataWrapper.WeaponFirearmItems.Add(ItemID, FWeaponFirearmItemsSaveArray());

			FWeaponFirearmSaveData WeaponItemSaveData = WeaponItem->CreateWeaponFirearmSaveData();
			SaveDataWrapper.WeaponFirearmItems[ItemID].WeaponFirearmItems.Add(WeaponItemSaveData);
			RemaingItemsToSave.Remove(Item);
			continue;
		}

		if (Item->IsA(UBWWeaponItem::StaticClass()))
		{
			const UBWWeaponItem* WeaponItem = Cast<UBWWeaponItem>(Item);
			FGuid ItemID = Item->GetItemData()->ItemDataID;
			
			if (!SaveDataWrapper.GenericWeaponItems.Contains(ItemID))
				SaveDataWrapper.GenericWeaponItems.Add(ItemID, FWeaponItemsSaveArray());

			FWeaponItemSaveData WeaponItemSaveData = WeaponItem->CreateWeaponItemSaveData();
			SaveDataWrapper.GenericWeaponItems[ItemID].WeaponItems.Add(WeaponItemSaveData);
			RemaingItemsToSave.Remove(Item);
		}
	}

	ItemsToSave = RemaingItemsToSave;
	SaveDataWrapper.AmmoSaveData = AmmoInventory->CreateAmmoSaveData();
}

void UBWInventory::LoadBWInventorySaveData(FBWInventorySaveDataWrapper& SaveDataWrapper)
{
	for (auto& WeaponFirearmItems : SaveDataWrapper.WeaponFirearmItems)
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
	
	for (auto& WeaponItems : SaveDataWrapper.GenericWeaponItems)
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

	AmmoInventory->LoadAmmoSaveData(SaveDataWrapper.AmmoSaveData);
}
