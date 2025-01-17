// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/ClassicInventory/ClassicInventory.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/ClassicItem.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/Data/SaveData/ClassicInventorySaveData.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/Data/ClassicItemSlotData.h"
#include "Features/Gameplay/InventorySystem/ClassicInventory/Data/SaveData/ClassicItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

class UClassicInventorySaveData;

UClassicInventory::UClassicInventory()
{
}

USaveData* UClassicInventory::CreateSaveData()
{
	UClassicInventorySaveData* ClassicInventorySaveData = NewObject<UClassicInventorySaveData>();
	
	for (UItemBase* Item : GetItems())
	{
		const UClassicItem* ClassicItem = Cast<UClassicItem>(Item);
		FClassicItemSaveData ItemSaveData = ClassicItem->CreateClassicItemSaveData();
		FGuid ItemID = Item->GetItemData()->ItemDataID;
		ClassicInventorySaveData->SavedItems.Add(ItemID, ItemSaveData);
	}

	return ClassicInventorySaveData;
}

void UClassicInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	const UClassicInventorySaveData* ClassicInventorySaveData = Cast<UClassicInventorySaveData>(InventorySaveData);
	
	for (const auto& LoadedItem : ClassicInventorySaveData->SavedItems)
	{
		const FGuid ItemID = LoadedItem.Key;
		const FClassicItemSaveData ItemSaveData = LoadedItem.Value;
	
		if (UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID))
		{
			UClassicItem* ClassicItem = Cast<UClassicItem>(UISFactory::CreateItem(ItemData));
			ClassicItem->LoadClassicItemSaveData(this, ItemSaveData);
		}
		else
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("Item with ID %s not found in the inventory registry %s."), *ItemID.ToString(), *GetName());
		}
	}
}

void UClassicInventory::OnItemAdded_Implementation(UItemBase* Item)
{
	if (UItemBase* FoundItem = Find(Item->GetItemData()))
	{
		UClassicItem* ClassicItem = Cast<UClassicItem>(FoundItem);
		ClassicItem->IncreaseQuantity(); // Increase quantity of the item, if it already exists in the inventory
	}
}

int32 UClassicInventory::GetTotalMinRequiredSlotsCount() const
{
	int32 CurrSlots = 0;
	
	for (const UItemBase* Item : Items)
	{
		const UClassicItem* ClassicItem = Cast<UClassicItem>(Item);
		CurrSlots += ClassicItem->GetMinRequiredSlots();
	}
	
	return CurrSlots;
}

bool UClassicInventory::IsFull() const
{
	if (IsSatisfyingAllRequiredSlots()) return false;

	for (const UItemBase* Item : Items)
	{
		const UClassicItem* ClassicItem = Cast<UClassicItem>(Item);
		for (const FClassicItemSlotData& Slot : ClassicItem->GetRequiredSlots()) // For each slot the item occupies
		{
			if (!Slot.IsMaxStacked())
				return false;
		}
	}

	return true;
}

bool UClassicInventory::IsSatisfyingAllRequiredSlots() const
{
	return GetTotalMinRequiredSlotsCount() < Capacity;
}

bool UClassicInventory::CanContainItem(const UItemDataBase* ItemData) const
{
	if (!UISInventoriesUtility::IsItemInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is not registered in the inventory registry %s."), *ItemData->ItemTypeID, *GetName());
		return false;
	}
	
	if (IsFull())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, inventory %s is full."), *GetName());
		return false;
	}

	if (const UItemBase* FoundItem = Find(ItemData))
	{
		if (FoundItem->GetItemData()->bIsUnique)
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeID, *GetName());
			return false;
		}

		const UClassicItem* ClassicItem = Cast<UClassicItem>(FoundItem);
		for (const FClassicItemSlotData& Slot : ClassicItem->GetRequiredSlots())
		{
			if (!Slot.IsMaxStacked())
				return true;
		}
	}

	return IsSatisfyingAllRequiredSlots();
}
