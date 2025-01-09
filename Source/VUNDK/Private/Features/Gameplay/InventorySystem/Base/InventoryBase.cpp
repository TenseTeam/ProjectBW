// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/InventoryBaseSaveData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UInventoryBase::UInventoryBase(): RelatedEquipment(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
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
		// Clear the inventory before loading the save data
		if (IsValid(RelatedEquipment))
			RelatedEquipment->ClearEquipment();

		ClearInventory();
		LoadInventorySaveData(InventorySaveData);
		return true;
	}

	UE_LOG(LogInventorySystem, Error, TEXT("Invalid Save Data type for inventory %s."), *GetName());
	return false;
}

void UInventoryBase::LinkEquipment(UEquipment* Equipment)
{
	RelatedEquipment = Equipment;
}

void UInventoryBase::UnlinkEquipment()
{
	RelatedEquipment = nullptr;
}

UEquipment* UInventoryBase::GetEquipment() const
{
	return RelatedEquipment;
}

UItemBase* UInventoryBase::AddNewItem(UItemDataBase* ItemData, int32& OutOverflow, const int32 Amount)
{
	OutOverflow = Amount;

	if (ItemData == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("AddNewItem(), Cannot add Item, ItemData is null."));
		return nullptr;
	}

	UItemBase* NewItem = nullptr;
	if (TryIncreaseAvailableStackForNewItem(ItemData, Amount, NewItem, OutOverflow))
		return NewItem;

	if (!CanContainItem(ItemData))
		return nullptr;

	UE_LOG(LogInventorySystem, Display, TEXT("Adding Item %s to inventory %s."), *ItemData->ItemTypeID, *GetName())
	int32 Overflow = 0;
	NewItem = UISFactory::CreateItemInQuantity(ItemData, Overflow, Amount);
	OutOverflow = Overflow;

	if (NewItem == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to add Item %s to inventory %s"), *ItemData->ItemTypeID, *GetName());
		return nullptr;
	}

	AddItemToList(NewItem);
	OnItemAdded(NewItem);

	if (Overflow > 0)
	{
		if (UItemBase* NewOverflowItem = AddNewItem(ItemData, OutOverflow, Overflow); NewOverflowItem != nullptr)
			return NewOverflowItem;
	}

	return NewItem;
}

bool UInventoryBase::TryAddItem(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("AddItem(), Cannot add Item, Item is null."));
		return false;
	}

	const UItemDataBase* ItemData = Item->GetItemData();

	if (TryIncreaseAvailableStackForItem(Item))
		return true;

	if (!CanContainItem(ItemData))
		return false;

	AddItemToList(Item);
	OnItemAdded(Item);
	return true;
}

bool UInventoryBase::RemoveItemByDataID(UItemDataBase* ItemData)
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
	if (!IsValid(Item) || !Items.Contains(Item))
		return;

	OnItemRemoved(Item);
	RemoveItemFromList(Item);
}

void UInventoryBase::ConsumeItem(UItemDataBase* ItemData, const int32 AmountToConsume, const bool bForceConsume) const
{
	if (!IsValid(ItemData) || AmountToConsume <= 0)
		return;

	auto ConsumeIteratively = [this](const UItemDataBase* ItemDataToFind, const int32 Amount)
	{
		int32 Remaining = Amount;

		while (Remaining > 0)
		{
			UItemBase* FoundItem = Find(ItemDataToFind);
			if (!IsValid(FoundItem))
				break;

			Remaining = FoundItem->Consume(Remaining);
		}
	};

	if (bForceConsume)
	{
		ConsumeIteratively(ItemData, AmountToConsume);
		return;
	}

	if (HasEnoughQuantity(ItemData, AmountToConsume))
		ConsumeIteratively(ItemData, AmountToConsume);
}

void UInventoryBase::ClearInventory()
{
	if (Items.Num() == 0 || IsEmpty()) // Items.Num == 0 is a safe check, sine IsEmpty() is a virtual function
		return;
	
	Items.Empty();
	OnClearedInventory();
	OnInventoryModified.Broadcast();
	OnInventoryCleared.Broadcast();
}

UItemBase* UInventoryBase::Find(const UItemDataBase* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			return Item;
	}

	return nullptr;
}

bool UInventoryBase::IsFull() const
{
	return Items.Num() >= Capacity;
}

bool UInventoryBase::IsCompletelyFull() const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->CanStack())
			return false;
	}

	return IsFull();
}

bool UInventoryBase::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventoryBase::CanContainItem(const UItemDataBase* ItemData) const
{
	if (!UISInventoriesUtility::IsItemInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Cannot add Item, %s is not registered in the inventory registry %s."),
		       *ItemData->ItemTypeID, *GetName());
		return false;
	}

	if (ItemData->bIsUnique && HasItemOfTypeID(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning,
		       TEXT("Cannot add Item, %s is unique and already exists in the inventory %s."), *ItemData->ItemTypeID,
		       *GetName());
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

bool UInventoryBase::HasItemOfDataID(const UItemDataBase* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			return true;
	}

	return false;
}

bool UInventoryBase::HasEnoughQuantity(const UItemDataBase* ItemData, const int32 Quantity) const
{
	if (!IsValid(ItemData))
		return false;

	if (Quantity <= 0)
		return true;

	int32 FoundQuantity = 0;

	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID)
			FoundQuantity += Item->GetCurrentQuantity();
	}

	return FoundQuantity >= Quantity;
}

UItemDataBase* UInventoryBase::GetItemDataFromRegistry(const FName ItemDataID) const
{
	return UISInventoriesUtility::GetItemDataFromRegistry(ItemDataID);
}

TArray<UItemBase*> UInventoryBase::GetItems(const bool bIncludeEquippedItems)
{
	TSet<UItemBase*> AllItems = TSet<UItemBase*>(Items);

	if (bIncludeEquippedItems && RelatedEquipment != nullptr)
		AllItems.Append(RelatedEquipment->GetEquippedItems());

	return AllItems.Array();
}

void UInventoryBase::AddItemToList(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("AddItemToList(), Cannot add Item, Item is null."));
		return;
	}

	Item->AssignInventory(this);
	Items.Add(Item);
	Item->OnAdd();
	Item->OnItemAdded.Broadcast(Item, this);
	OnAnyItemAdded.Broadcast(Item);
	OnInventoryModified.Broadcast();
}

void UInventoryBase::RemoveItemFromList(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("RemoveItemFromList(), Cannot remove Item, Item is null."));
		return;
	}

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

bool UInventoryBase::TryIncreaseAvailableStackForNewItem(UItemDataBase* ItemData, const int32 Amount, UItemBase*& OutFoundItem, int32& OutOverflow)
{
	if (UItemBase* FoundItem = FindAvailableStackForNewItem(ItemData); IsValid(FoundItem))
	{
		const int32 Overflow = FoundItem->IncreaseQuantity(Amount);
		OutFoundItem = FoundItem;

		if (Overflow > 0)
		{
			AddNewItem(ItemData, OutOverflow, Overflow);
		}

		return true;
	}

	return false;
}

bool UInventoryBase::TryIncreaseAvailableStackForItem(UItemBase* Item) const
{
	if (UItemBase* FoundItemStack = FindAvailableStackForItem(Item); IsValid(FoundItemStack))
	{
		const int32 Rest = FoundItemStack->IncreaseQuantity(Item->GetCurrentQuantity());
		Item->SetQuantity(Rest);
		return true;
	}

	return false;
}

UItemBase* UInventoryBase::FindAvailableStackForNewItem(const UItemDataBase* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->ItemDataID == ItemData->ItemDataID && Item->CanStack())
			return Item;
	}

	return nullptr;
}

UItemBase* UInventoryBase::FindAvailableStackForItem(UItemBase* Item) const
{
	for (UItemBase* StackableItem : Items)
	{
		if (StackableItem->CanStackItem(Item))
			return StackableItem;
	}

	return nullptr;
}
