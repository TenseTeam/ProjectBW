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

	EquipSlots = TMap<FName, TMap<int32, UItemBase*>>();
	for (const auto& SlotLimit : SlotLimits)
	{
		if (SlotLimit.Key == nullptr)
			continue;

		TMap<int32, UItemBase*> IndexedItems = TMap<int32, UItemBase*>();

		for (int32 i = 0; i < SlotLimit.Value; i++)
			IndexedItems.Add(i, nullptr);

		EquipSlots.Add(SlotLimit.Key->EquipSlotKey, IndexedItems);
	}
}

bool UEquipment::TryEquipItemToFirstAvailableSlot(UItemBase* Item)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()))
		return false;

	UEquipSlotKey* ItemSlotKey = Item->GetItemData()->EquipSlotKey;

	if (!EquipSlots.Contains(ItemSlotKey->EquipSlotKey))
		return false;

	for (int32 i = 0; i < SlotLimits[ItemSlotKey]; i++)
	{
		if (TryEquipItem(Item, ItemSlotKey, i))
			return true;
	}

	return false;
}

bool UEquipment::TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()) || !IsValid(TargetSlotKey))
	{
		UE_LOG(LogEquipment, Warning, TEXT("UEquipment::TryEquipItem: Item or TargetSlotKey is not valid."));
		return false;
	}

	UEquipSlotKey* EquipSlotKey = Item->GetItemData()->EquipSlotKey;

	if (!CanEquipItem(Item, EquipSlotKey, SlotIndex))
	{
		UE_LOG(LogEquipment, Warning, TEXT("UEquipment::TryEquipItem: Item cannot be equipped."));
		return false;
	}

	if (Item->IsEquipped())
	{
		ChangeItemEquipSlot(Item, EquipSlotKey, SlotIndex); // If the item is already equipped, change the slot
		return true;
	}

	EquipItem(Item, EquipSlotKey, SlotIndex, bRemoveFromInventory);
	OnAnyItemEquipped.Broadcast(EquipSlotKey, SlotIndex, Item);
	OnEquipChanged.Broadcast();
	return true;
}

bool UEquipment::TryUnequipItem(UItemBase* Item)
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()))
		return false;

	UEquipSlotKey* EquipSlotKey = Item->GetItemData()->EquipSlotKey;
	const TMap<int32, UItemBase*> SlotItems = EquipSlots[EquipSlotKey->EquipSlotKey];
	const int32* SlotIndexRef = SlotItems.FindKey(Item);
	
	if (!EquipSlots.Contains(EquipSlotKey->EquipSlotKey) || SlotIndexRef == nullptr)
		return false;
	
	UnequipItem(Item, EquipSlotKey, *SlotIndexRef);
	OnAnyItemUnequipped.Broadcast(EquipSlotKey, *SlotIndexRef, Item);
	OnEquipChanged.Broadcast();
	return true;
}

TSet<UItemBase*> UEquipment::GetEquippedItems()
{
	TSet<UItemBase*> Items;
	for (const auto& SlotItems : EquipSlots)
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
	for (auto& SlotItems : EquipSlots)
	{
		for (auto& Item : SlotItems.Value)
			Item.Value = nullptr;
	}

	OnEquipmentCleared.Broadcast();
	OnEquipChanged.Broadcast();
}

bool UEquipment::CanEquipItem(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const
{
	if (!IsValid(Item) || !IsValid(Item->GetItemData()) || !IsValid(TargetSlotKey))
		return false;

	if (Item->GetItemData()->EquipSlotKey != TargetSlotKey || !EquipSlots.Contains(TargetSlotKey->EquipSlotKey))
		return false;

	TMap<int32, UItemBase*> SlotItems = EquipSlots[TargetSlotKey->EquipSlotKey];

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

void UEquipment::EquipItem(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory)
{
	AddItemInEquipSlot(Item, EquipSlotKey, SlotIndex);

	if (bRemoveFromInventory)
		Item->Remove();
	
	Item->OnItemAdded.AddDynamic(this, &UEquipment::OnItemAddedToAnyInventory); // If the item is added to any inventory or equipped to another slot, remove it from the current slot
}

void UEquipment::UnequipItem(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex)
{
	RemoveItemFromEquipSlot(Item, EquipSlotKey, SlotIndex);
	Item->OnItemAdded.RemoveDynamic(this, &UEquipment::OnItemAddedToAnyInventory);
}

void UEquipment::AddItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	EquipSlots[EquipSlotKey->EquipSlotKey][NewSlotIndex] = Item;
	Item->SetEquipSlot(this, NewSlotIndex);
}

void UEquipment::RemoveItemFromEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex)
{
	EquipSlots[EquipSlotKey->EquipSlotKey][SlotIndex] = nullptr;
	Item->ClearEquipSlot();
}

void UEquipment::ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex)
{
	const TMap<int32, UItemBase*> SlotItems = EquipSlots[EquipSlotKey->EquipSlotKey];
	const int32* OldSlotIndexRef = SlotItems.FindKey(Item);

	if (OldSlotIndexRef == nullptr) // Safe check to prevent crashes
	{
		EquipItem(Item, EquipSlotKey, NewSlotIndex);
		return;
	}

	UnequipItem(Item, EquipSlotKey, *OldSlotIndexRef);
	EquipItem(Item, EquipSlotKey, NewSlotIndex);
	OnEquipChanged.Broadcast();
	OnAnyItemEquipSlotChanged.Broadcast(EquipSlotKey, Item, NewSlotIndex, *OldSlotIndexRef);
}

void UEquipment::OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory)
{
	TryUnequipItem(Item);
}
