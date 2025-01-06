// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/EquipmentSystem/Equipment.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"

UEquipment::UEquipment()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipment::BeginPlay()
{
	Super::BeginPlay();

	EquippedItems = TMap<FName, TMap<int32, UItemBase*>>();
	for (const auto& SlotLimit : SlotLimits)
	{
		if (SlotLimit.Key == nullptr)
			continue;
		
		TMap<int32, UItemBase*> IndexedItems = TMap<int32, UItemBase*>();

		for (int32 i = 0; i < SlotLimit.Value; i++)
			IndexedItems.Add(i, nullptr);
		
		EquippedItems.Add(SlotLimit.Key->EquipSlotKey, IndexedItems);
	}
}

bool UEquipment::TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()) || !IsValid(TargetSlotKey))
		return false;

	UEquipSlotKey* ItemSlotKey = Item->GetItemData()->EquipSlotKey;

	if (!CanEquipItem(Item, ItemSlotKey, SlotIndex))
		return false;

	if (Item->IsEquipped())
	{
		ChangeItemEquipSlot(Item, ItemSlotKey, SlotIndex); // If the item is already equipped, unequip it first
		return true;
	}
	
	EquipItem(Item, ItemSlotKey, SlotIndex, bRemoveFromInventory);
	return true;
}

bool UEquipment::TryUnequipItem(UItemBase* Item)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()))
		return false;
	
	UEquipSlotKey* ItemSlotKey = Item->GetItemData()->EquipSlotKey;

	if (!EquippedItems.Contains(ItemSlotKey->EquipSlotKey) || EquippedItems[ItemSlotKey->EquipSlotKey].FindKey(Item) == nullptr)
		return false;

	UnequipItem(Item, ItemSlotKey);
	return true;
}

TSet<UItemBase*> UEquipment::GetEquippedItems()
{
	TSet<UItemBase*> Items;
	for (const auto& SlotItems : EquippedItems)
	{
		for (const auto& Item : SlotItems.Value)
		{
			if (Item.Value != nullptr)
				Items.Add(Item.Value);
		}
	}
	
	return Items;
}

void UEquipment::ClearEquipment()
{
	for (auto& SlotItems : EquippedItems)
	{
		for (auto& Item : SlotItems.Value)
			Item.Value = nullptr;
	}

	OnEquipmentCleared.Broadcast();
}

bool UEquipment::CanEquipItem(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const
{
	if (Item->GetItemData()->EquipSlotKey != TargetSlotKey || !EquippedItems.Contains(TargetSlotKey->EquipSlotKey))
		return false;

	TMap<int32, UItemBase*> SlotItems = EquippedItems[TargetSlotKey->EquipSlotKey];
	
	const bool bIsSlotIndexValid = SlotIndex >= 0 && SlotIndex < SlotLimits[TargetSlotKey];
	const bool bIsSlotEmpty = SlotItems.Contains(SlotIndex) && SlotItems[SlotIndex] == nullptr;
	
	return bIsSlotIndexValid && bIsSlotEmpty;
}

void UEquipment::OnEquipItem_Implementation(UEquipSlotKey* EquipSlotKey, UItemBase* Item)
{
}

void UEquipment::OnUnequipItem_Implementation(UEquipSlotKey* EquipSlotKey, UItemBase* Item)
{
}

void UEquipment::EquipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory)
{
	SetItemInEquipSlot(Item, EquipSlotKey, SlotIndex);
	Item->Equip(SlotIndex);
	OnAnyItemEquipped.Broadcast(EquipSlotKey, SlotIndex, Item);
	
	// If the item is added to any inventory or equipped to another slot, remove it from the current slot
	Item->OnItemAdded.AddDynamic(this, &UEquipment::OnItemAddedToAnyInventory);
	
	if (bRemoveFromInventory)
		Item->Remove();
}

void UEquipment::UnequipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey)
{
	const TMap<int32, UItemBase*> SlotItems = EquippedItems[EquipSlotKey->EquipSlotKey];
	const int32 Index = *(SlotItems.FindKey(Item));

	SetItemInEquipSlot(nullptr, EquipSlotKey, Index);
	Item->Unequip();
	OnAnyItemUnequipped.Broadcast(EquipSlotKey, Index, Item);
	Item->OnItemAdded.RemoveDynamic(this, &UEquipment::OnItemAddedToAnyInventory);
}

void UEquipment::SetItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	EquippedItems[EquipSlotKey->EquipSlotKey][NewSlotIndex] = Item;
}

void UEquipment::ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	const TMap<int32, UItemBase*> SlotItems = EquippedItems[EquipSlotKey->EquipSlotKey];
	const int32* OldSlotKeyRef = SlotItems.FindKey(Item);
	
	if (OldSlotKeyRef == nullptr) // Safe check to prevent crashes
	{
		EquipItem(Item, EquipSlotKey, NewSlotIndex);
		return;
	}
	
	SetItemInEquipSlot(nullptr, EquipSlotKey, *OldSlotKeyRef);
	SetItemInEquipSlot(Item, EquipSlotKey, NewSlotIndex);
	Item->Equip(NewSlotIndex);

	OnAnyItemEquipSlotChanged.Broadcast(EquipSlotKey, Item, NewSlotIndex, *OldSlotKeyRef);
}

void UEquipment::OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory)
{
	TryUnequipItem(Item);
}
