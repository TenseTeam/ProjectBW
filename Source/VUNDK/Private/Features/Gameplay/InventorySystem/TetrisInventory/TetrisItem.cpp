// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventory.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/TetrisItemData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/SaveData/TetrisItemSaveData.h"

UTetrisItem::UTetrisItem(): TopLeftCornerPosition(),
                            bIsRotated(false),
                            CachedSize(),
                            bCachedRotation(false)
{
}

void UTetrisItem::Init(UItemDataBase* Data)
{
	Super::Init(Data);
	CachedSize = GetTetrisItemData()->Size;
}

FTetrisItemSaveData UTetrisItem::CreateTetrisSaveData() const
{
	FTetrisItemSaveData TetrisSaveData;
	TetrisSaveData.ItemSaveData = CreateItemBaseSaveData();
	TetrisSaveData.SlotPosition = TopLeftCornerPosition;
	TetrisSaveData.bIsRotated = bIsRotated;
	return TetrisSaveData;
}

void UTetrisItem::LoadTetrisSaveData(UInventoryBase* LoadingInventory, const FTetrisItemSaveData& TetrisSaveData)
{
	bool bHasBeenEquipped;
	LoadItemBaseSaveData(LoadingInventory, TetrisSaveData.ItemSaveData, bHasBeenEquipped);
	UTetrisInventory* TetrisInventory = Cast<UTetrisInventory>(LoadingInventory);

	if (bHasBeenEquipped)
		return;

	SetRotation(TetrisSaveData.bIsRotated);
	TetrisInventory->TryMoveItem(this, TetrisSaveData.SlotPosition); // Since the item is already added in LoadItemBaseSaveData
}

void UTetrisItem::SetRotation(const bool bNewRotation)
{
	if (!GetTetrisItemData()->bCanBeRotated || GetTetrisItemData()->Size.X == GetTetrisItemData()->Size.Y)
		return;

	bIsRotated = bNewRotation;
	OnItemRotated.Broadcast(GetRelativeSize());
}

void UTetrisItem::ResetToCachedRotation()
{
	SetRotation(bCachedRotation);
}

void UTetrisItem::Rotate()
{
	SetRotation(!bIsRotated);
}

UTetrisItemData* UTetrisItem::GetTetrisItemData() const
{
	return Cast<UTetrisItemData>(ItemData);
}

FIntPoint UTetrisItem::GetCurrentPosition() const
{
	return TopLeftCornerPosition;
}

FIntPoint UTetrisItem::GetCachedSize() const
{
	return CachedSize;
}

bool UTetrisItem::GetCachedRotation() const
{
	return bCachedRotation;
}

FIntPoint UTetrisItem::GetRelativeSize() const
{
	const FIntPoint Size = GetTetrisItemData()->Size;
	return bIsRotated ? FIntPoint(Size.Y, Size.X) : Size;
}

bool UTetrisItem::IsRotated() const
{
	return bIsRotated;
}

void UTetrisItem::SetCurrentPosition(const FIntPoint NewPosition)
{
	TopLeftCornerPosition = NewPosition;
}

void UTetrisItem::CacheCurrentRotation()
{
	bCachedRotation = bIsRotated;
	CachedSize = GetRelativeSize();
}

void UTetrisItem::OnEquip_Implementation()
{
	Super::OnEquip_Implementation();
	SetRotation(false);
}
