// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/ItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Kismet/GameplayStatics.h"

UItemBase::UItemBase(): RelatedInventory(nullptr),
                        ItemData(nullptr),
                        RelatedEquipment(nullptr),
                        EquipSlotIndex(-1),
                        CurrentQuantity(1)
{
}

FItemSaveData UItemBase::CreateItemBaseSaveData() const
{
	FItemSaveData ItemSaveData;
	ItemSaveData.EquipSlotIndex = EquipSlotIndex;
	ItemSaveData.Quantity = CurrentQuantity;
	return ItemSaveData;
}

void UItemBase::LoadItemBaseSaveData(UInventoryBase* LoadingInventory, const FItemSaveData ItemSaveData, bool& bOutHasBeenEquipped)
{
	bOutHasBeenEquipped = false;
	EquipSlotIndex = ItemSaveData.EquipSlotIndex;
	CurrentQuantity = ItemSaveData.Quantity;

	if (EquipSlotIndex == -1)
	{
		if (!LoadingInventory->TryAddItem(this))
			UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::LoadItemBaseSaveData, Item %s couldn't be added to the inventory %s."), *ItemData->ItemTypeID, *LoadingInventory->GetName());
		
		return;
	}
	
	if (!LoadingInventory->GetEquipment()->TryEquipItem(this, ItemData->EquipSlotKey, EquipSlotIndex))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::LoadItemBaseSaveData, Item %s couldn't be equipped to the equipment %s."), *ItemData->ItemTypeID, *LoadingInventory->GetEquipment()->GetName());
		ClearEquipSlot();
		return;
	}

	bOutHasBeenEquipped = true;
}

void UItemBase::Init(UItemDataBase* Data)
{
	OnPreInit(Data);
	ItemData = Data;
	OnInit();
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init, Item %s has been initialized."), *ItemData->ItemTypeID);
}

UItemDataBase* UItemBase::GetItemData() const
{
	return ItemData;
}

void UItemBase::AssignInventory(UInventoryBase* Inventory)
{
	RelatedInventory = Inventory;
}

void UItemBase::DeassignInventory()
{
	RelatedInventory = nullptr;
}

void UItemBase::SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex)
{
	RelatedEquipment = InEquipment;
	EquipSlotIndex = SlotIndex;
	OnItemEquipped.Broadcast(this);
	OnEquip();
}

void UItemBase::ClearEquipSlot()
{
	RelatedEquipment = nullptr;
	EquipSlotIndex = -1;
	OnItemUnequipped.Broadcast(this);
	OnUnequip();
}

bool UItemBase::IsEquipped() const
{
	return IsValid(RelatedEquipment) && EquipSlotIndex > -1;
}

bool UItemBase::TryDrop(APlayerController* PlayerController, const FVector Location, const FRotator Rotation)
{
	if (!CanDrop())
		return false;
	
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::Drop(), PlayerController is nullptr."));
		return false;
	}
	
	AItemDropActor* ItemDropActor = UISFactory::CreateItemDropActor(this, PlayerController);
	
	if (ItemDropActor == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("ItemBase::Drop(), ItemDropActor is nullptr."));
		return false;
	}

	ItemDropActor->SetActorLocationAndRotation(Location, Rotation);
	Remove();

	if (IsEquipped())
		RelatedEquipment->TryUnequipItem(this);
	
	return true;
}

void UItemBase::Remove()
{
	if (!IsValid(RelatedInventory))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove(), RelatedInventory is not valid."));
		return;
	}

	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove(), Item %s has been removed from the inventory %s."), *ItemData->ItemTypeID, *RelatedInventory->GetName());
	RelatedInventory->RemoveItem(this);
}

void UItemBase::Use()
{
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Use(), Item %s has been used."), *ItemData->ItemTypeID);

	OnItemUsed.Broadcast(this);
	RelatedInventory->OnAnyItemUsed.Broadcast(this);
	OnUse();

	if (ItemData->bConsumeUponUse)
		Consume();
}

int32 UItemBase::Consume(const int32 AmountToConsume)
{
	if (!ItemData->bIsConsumable)
		return 0;

	UE_LOG(LogInventorySystem, Display, TEXT("Consuming Item %s, Current Quantity: %d, Amount To Consume: %d."), *ItemData->ItemTypeID, CurrentQuantity, AmountToConsume);

	RelatedInventory->OnAnyItemConsumed.Broadcast(this);
	OnItemConsumed.Broadcast(this);
	OnConsume_Implementation();
	return DecreaseQuantity(AmountToConsume);
}

FText UItemBase::GetItemFullName() const
{
	return ItemData->ItemName;
}

FSlateBrush UItemBase::GetItemIcon() const
{
	return ItemData->ItemIcon;
}

FText UItemBase::GetItemDescription() const
{
	return ItemData->ItemDescription;
}

UStaticMesh* UItemBase::GetItemMesh() const
{
	return ItemData->ItemMesh;
}

int32 UItemBase::GetEquipSlotIndex() const
{
	return EquipSlotIndex;
}

bool UItemBase::CanStack() const
{
	return IsStackable() && !IsMaxStacked();
}

bool UItemBase::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	return
		OtherItem != this &&
		IsValid(OtherItem) &&
		OtherItem->ItemData == ItemData &&
		CanStack();
}

bool UItemBase::IsMaxStacked() const
{
	return CurrentQuantity >= ItemData->MaxStackSize;
}

bool UItemBase::IsStackable() const
{
	return ItemData->MaxStackSize > 1;
}

int32 UItemBase::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

bool UItemBase::TryStackItem(UItemBase* Item, const int32 AmountToStack)
{
	if (AmountToStack <= 0 || !CanStackItem(Item))
		return false;

	if (Item->GetCurrentQuantity() - AmountToStack < 0 || GetCurrentQuantity() + AmountToStack > ItemData->MaxStackSize)
		return false;

	IncreaseQuantity(AmountToStack);
	Item->DecreaseQuantity(AmountToStack);

	return true;
}

bool UItemBase::TrySplitItem(const int32 AmountToSplit, UItemBase*& OutNewSplittedItem)
{
	OutNewSplittedItem = nullptr;
	
	if (AmountToSplit <= 0 || AmountToSplit >= CurrentQuantity || !IsStackable())
		return false;

	OutNewSplittedItem = DuplicateItem();
	if (OutNewSplittedItem == nullptr)
		return false;
	
	OutNewSplittedItem->SetQuantity(AmountToSplit);
	DecreaseQuantity(AmountToSplit);
	
	return true;
}

int32 UItemBase::IncreaseQuantity(const int32 Amount)
{
	if (!CanStack())
		return 0;

	if (CurrentQuantity + Amount > ItemData->MaxStackSize)
	{
		const int32 Overflow = CurrentQuantity + Amount - ItemData->MaxStackSize;
		CurrentQuantity = ItemData->MaxStackSize;
		OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
		return Overflow;
	}

	CurrentQuantity += Amount;
	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
	return 0;
}

int32 UItemBase::DecreaseQuantity(const int32 Amount)
{
	if (!IsStackable())
		return 0;
	
	if (CurrentQuantity - Amount > 0)
	{
		CurrentQuantity -= Amount;
		OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
		return 0;
	}

	const int32 Rest = Amount - CurrentQuantity;
	CurrentQuantity = 0;
	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);
	Remove();
	return Rest;
}

int32 UItemBase::SetQuantity(const int32 Quantity)
{
	if (!IsStackable())
		return 0;
	
	CurrentQuantity = FMath::Clamp(Quantity, 0, ItemData->MaxStackSize);
	
	if (CurrentQuantity <= 0)
		Remove();
	
	OnItemQuantityChanged.Broadcast(this, CurrentQuantity);

	int32 Rest = Quantity - ItemData->MaxStackSize;
	Rest = Rest < 0 ? 0 : Rest;
	return Rest;
}

UItemBase* UItemBase::DuplicateItem() const
{
	return DuplicateObject<UItemBase>(this, GetOuter());
}

void UItemBase::OnPreInit(UItemDataBase* Data)
{
}

void UItemBase::OnInit_Implementation()
{
}

void UItemBase::OnAdd_Implementation()
{
}

void UItemBase::OnRemove_Implementation()
{
}

void UItemBase::OnUse_Implementation()
{
}

void UItemBase::OnConsume_Implementation()
{
}

void UItemBase::OnEquip_Implementation()
{
}

void UItemBase::OnUnequip_Implementation()
{
}

AGameModeBase* UItemBase::GetGameMode() const
{
	return UGameplayStatics::GetGameMode(RelatedInventory);
}

APlayerController* UItemBase::GetPlayerController(const int PlayerIndex) const
{
	return UGameplayStatics::GetPlayerController(RelatedInventory, PlayerIndex);
}

APlayerState* UItemBase::GetPlayerState(const int PlayerStateIndex) const
{
	return UGameplayStatics::GetPlayerState(RelatedInventory, PlayerStateIndex);
}

bool UItemBase::CanDrop() const
{
	return ItemData->ItemDropActorClass != nullptr;
}
