// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/EquipmentSystem/EquipmentManager.h"
#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/SaveData/ItemSaveData.h"
#include "Kismet/GameplayStatics.h"

UItemBase::UItemBase()
{
	RelatedInventory = nullptr;
	ItemData = nullptr;
	EquipSlotIndex = -1;
}

FItemSaveData UItemBase::CreateItemBaseSaveData() const
{
	FItemSaveData ItemSaveData;
	ItemSaveData.EquipSlotIndex = EquipSlotIndex;
	return ItemSaveData;
}

void UItemBase::LoadItemBaseSaveData(const FItemSaveData ItemSaveData)
{
	EquipSlotIndex = ItemSaveData.EquipSlotIndex;
}

void UItemBase::Init(UItemDataBase* Data)
{
	ItemData = Data;
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init, Item %s has been initialized."), *ItemData->ItemTypeID);
	OnPreInit(Data);
	OnInit();
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

void UItemBase::Equip(const int32 SlotIndex)
{
	EquipSlotIndex = SlotIndex;
	OnItemEquipped.Broadcast(this);
	OnEquip();
}

void UItemBase::Unequip()
{
	EquipSlotIndex = -1;
	OnItemUnequipped.Broadcast(this);
	OnUnequip();
}

void UItemBase::Remove()
{
	if (RelatedInventory == nullptr)
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

	if (ItemData->bIsConsumable)
		Consume();
}

void UItemBase::Consume()
{
	UE_LOG(LogInventorySystem, Display, TEXT("Item %s has been consumed."), *ItemData->ItemTypeID);
	RelatedInventory->OnAnyItemConsumed.Broadcast(this);
	OnItemConsumed.Broadcast(this);
	OnConsume_Implementation();
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

bool UItemBase::IsEquipped() const
{
	return EquipSlotIndex > -1;
}

int32 UItemBase::GetEquipSlotIndex() const
{
	return EquipSlotIndex;
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
	Remove();
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
