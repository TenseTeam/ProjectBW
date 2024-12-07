// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"

#include "Features/Gameplay/EquipmentSystem/EquipmentManager.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/InventoryBaseSaveData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UInventoryBase::UInventoryBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	RelatedEquipment = nullptr;
}

USaveData* UInventoryBase::CreateSaveData()
{
	UE_LOG(LogInventorySystem, Display, TEXT("Creating Save Data for inventory %s."), *GetName());
	UInventoryBaseSaveData* InventorySaveData = NewObject<UInventoryBaseSaveData>();
	return Cast<USaveData>(InventorySaveData);
}

bool UInventoryBase::LoadSaveData(USaveData* SavedData)
{
	if (UInventoryBaseSaveData* InventorySaveData = Cast<UInventoryBaseSaveData>(SavedData))
	{
		LoadInventorySaveData(InventorySaveData);

		TArray<UItemBase*> EquippedItems = TSet<UItemBase*>(Items).Array();
		EquippedItems.RemoveAll([this](const UItemBase* Item)
		{
			return !Item->IsEquipped();
		});
		
		// Load equipped items if there is an equipment
		if (RelatedEquipment != nullptr)
			RelatedEquipment->LoadEquippedItems(EquippedItems);
		
		return true;
	}

	UE_LOG(LogInventorySystem, Error, TEXT("Invalid Save Data type for inventory %s."), *GetName());
	return false;
}

void UInventoryBase::SetRelatedEquipment(UEquipment* Equipment)
{
	RelatedEquipment = Equipment;
}

UItemBase* UInventoryBase::AddNewItem(UItemDataBase* ItemData)
{
	if (ItemData == nullptr) return nullptr;
	if (!CanContainItem(ItemData)) return nullptr;

	UE_LOG(LogInventorySystem, Display, TEXT("Adding Item %s to inventory %s."), *ItemData->ItemTypeID, *GetName())
	UItemBase* NewItem = UISFactory::CreateItem(ItemData);
	
	if (NewItem == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to add Item %s to inventory %s"), *ItemData->ItemTypeID, *GetName());
		return nullptr;
	}

	AddItem(NewItem);
	return NewItem;
}

void UInventoryBase::AddItem(UItemBase* Item)
{
	if (Item == nullptr) return;
	if (!CanContainItem(Item->GetItemData())) return;

	AddItemToList(Item);
	OnItemAdded(Item);
}

bool UInventoryBase::RemoveItemOfType(UItemDataBase* ItemData)
{
	if (UItemBase* FoundItem = Find(ItemData))
	{
		RemoveItem(FoundItem);
		return true;
	}

	return false;
}

void UInventoryBase::RemoveItem(UItemBase* Item)
{
	if (Item == nullptr || !Items.Contains(Item))
		return;

	OnItemRemoved(Item);
	RemoveItemFromList(Item);
}

void UInventoryBase::ClearInventory()
{
	Items.Empty();
	OnClearedInventory();
	OnInventoryModified.Broadcast();
	OnInventoryCleared.Broadcast();
}

UItemBase* UInventoryBase::Find(const UItemDataBase* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->GetItemDataID() == ItemData->GetItemDataID())
			return Item;
	}

	return nullptr;
}

bool UInventoryBase::IsFull() const
{
	return Items.Num() >= Capacity;
}

bool UInventoryBase::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventoryBase::CanContainItem(const UItemDataBase* ItemData) const
{
	if (!UISInventoriesUtility::IsItemInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is not registered in the inventory registry %s."), *ItemData->ItemTypeID, *GetName());
		return false;
	}

	if (ItemData->bIsUnique && HasItemOfTypeID(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeID, *GetName());
		return false;
	}
	
	if (IsFull())
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, inventory %s is full."), *GetName());
		return false;
	}

	return true;
}

bool UInventoryBase::HasItemOfTypeID(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemTypeID == ItemData->ItemTypeID)
			return true;
	}

	return false;
}

UItemDataBase* UInventoryBase::GetItemDataByID(const FName ItemDataID) const
{
	return UISInventoriesUtility::GetItemDataFromRegistry(ItemDataID);
}

TSet<UItemBase*> UInventoryBase::GetAllItems()
{
	TSet<UItemBase*> AllItems = TSet<UItemBase*>(Items);
	
	if (RelatedEquipment != nullptr)
		AllItems.Append(RelatedEquipment->GetEquippedItems());
	
	return AllItems;
}

void UInventoryBase::AddItemToList(UItemBase* Item)
{
	Item->AssignInventory(this);
	Items.Add(Item);
	Item->OnAdd();
	Item->OnItemAdded.Broadcast(Item, this);
	OnAnyItemAdded.Broadcast(Item);
	OnInventoryModified.Broadcast();
}

void UInventoryBase::RemoveItemFromList(UItemBase* Item)
{
	OnAnyItemRemoved.Broadcast(Item);
	Item->OnRemove();
	Item->OnItemRemoved.Broadcast(Item, this);
	Items.Remove(Item);

	if (Item->RelatedInventory == this)
		Item->DeassignInventory();

	OnInventoryModified.Broadcast();
}

void UInventoryBase::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
}

void UInventoryBase::OnItemAdded_Implementation(UItemBase* Item)
{
}

void UInventoryBase::OnItemRemoved_Implementation(UItemBase* Item)
{
}

void UInventoryBase::OnClearedInventory_Implementation()
{
}
