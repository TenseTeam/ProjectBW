// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventory.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/SaveData/TetrisInventorySaveData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/TetrisItemData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/Data/SaveData/TetrisItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"

UTetrisInventory::UTetrisInventory(): GridSize()
{
	PrimaryComponentTick.bCanEverTick = false;
}

USaveData* UTetrisInventory::CreateSaveData()
{
	UTetrisInventorySaveData* TetrisInventorySaveData = NewObject<UTetrisInventorySaveData>();

	for (UItemBase* Item : GetItems())
	{
		const UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);
		FTetrisItemSaveData ItemSaveData = TetrisItem->CreateTetrisSaveData();

		FName ItemID = Item->GetItemData()->ItemDataID;

		if (!TetrisInventorySaveData->TetrisItems.Contains(ItemID))
			TetrisInventorySaveData->TetrisItems.Add(ItemID, FTetrisItemsSaveArray());

		TetrisInventorySaveData->TetrisItems[ItemID].Items.Add(ItemSaveData);
	}

	return TetrisInventorySaveData;
}

void UTetrisInventory::LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData)
{
	UTetrisInventorySaveData* TetrisInventorySaveData = Cast<UTetrisInventorySaveData>(InventorySaveData);

	for (const TPair<FName, FTetrisItemsSaveArray>& LoadedItem : TetrisInventorySaveData->TetrisItems)
	{
		auto [ItemID, ItemsSaveArray] = LoadedItem;

		if (UItemDataBase* ItemData = GetItemDataFromRegistry(ItemID))
		{
			for (FTetrisItemSaveData& ItemSaveData : ItemsSaveArray.Items)
			{
				UTetrisItem* CreatedItem = Cast<UTetrisItem>(UISFactory::CreateItem(ItemData));
				CreatedItem->LoadTetrisSaveData(this, ItemSaveData);
			}
		}
		else
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("Item with ID %s not found in the inventory registry %s."), *ItemID.ToString(), *GetName());
		}
	}
}

bool UTetrisInventory::CanContainItem(const UItemDataBase* ItemData) const
{
	const UTetrisItemData* TetrisItemData = Cast<UTetrisItemData>(ItemData);

	bool bNeedsRotation;
	if (FIntPoint FoundSlot; !FindAvailableSlots(TetrisItemData->Size, FoundSlot, bNeedsRotation))
		return false;

	return Super::CanContainItem(ItemData);
}

bool UTetrisInventory::IsFull() const
{
	if (Super::IsFull()) return true;

	for (int32 x = 0; x < GridSize.X; x++)
	{
		for (int32 y = 0; y < GridSize.Y; y++)
		{
			if (!InvMatrix[x][y]->IsOccupied())
				return false;
		}
	}

	return true;
}

UTetrisSlot* UTetrisInventory::GetSlot(const FIntPoint SlotPosition) const
{
	if (!IsValidSlotPosition(SlotPosition))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid slot position."));
		return nullptr;
	}

	return InvMatrix[SlotPosition.X][SlotPosition.Y];
}

FIntPoint UTetrisInventory::GetGridSize() const
{
	return GridSize;
}

bool UTetrisInventory::CanOccupySlots(const FIntPoint Size, const FIntPoint StartSlotPosition, const UTetrisItem* IgnoreItem) const
{
	if (!IsValidSlotPosition(StartSlotPosition))
		return false;

	if (StartSlotPosition.X + Size.X > GridSize.X || StartSlotPosition.Y + Size.Y > GridSize.Y)
	{
		UE_LOG(LogTemp, Display, TEXT("Item size exceeds the inventory grid size."));
		return false;
	}

	for (int32 x = StartSlotPosition.X; x < StartSlotPosition.X + Size.X; x++)
	{
		for (int32 y = StartSlotPosition.Y; y < StartSlotPosition.Y + Size.Y; y++)
		{
			if (InvMatrix[x][y]->IsOccupied() && InvMatrix[x][y]->GetSlotItem() != IgnoreItem)
				return false;
		}
	}

	return true;
}

bool UTetrisInventory::TryAddNewItemAtSlots(UTetrisItemData* ItemData, const FIntPoint StartSlotPosition)
{
	if (ItemData == nullptr || !CanContainItem(ItemData))
		return false;

	UTetrisItem* Item = Cast<UTetrisItem>(UISFactory::CreateItem(ItemData));

	return TryOccupySlotsWithItem(Item, StartSlotPosition);
}

bool UTetrisInventory::TryAddItemAtSlots(UTetrisItem* Item, const FIntPoint StartSlotPosition)
{
	if (Item == nullptr || !CanContainItem(Item->GetItemData()))
		return false;

	return TryOccupySlotsWithItem(Item, StartSlotPosition);
}

bool UTetrisInventory::TryMoveItem(UTetrisItem* Item, const FIntPoint NewPosition)
{
	if (!IsValidSlotPosition(NewPosition))
		return false;

	const FIntPoint OldPosition = Item->GetCurrentPosition();
	FreeSlots(Item->GetCurrentPosition(), Item->GetCachedSize());

	if (!TryOccupySlotsWithItem(Item, NewPosition, false)) // Try to occupy the new slots
	{
		Item->ResetToCachedRotation();
		OccupySlotsWithItem(Item, OldPosition, false); // Re-occupy the old slots if failed
		return false;
	}

	OnItemMoved.Broadcast(Item, OldPosition, NewPosition);
	return true;
}

void UTetrisInventory::BeginPlay()
{
	Super::BeginPlay();
	ConstructGrid();
}

void UTetrisInventory::OnItemAdded_Implementation(UItemBase* Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("OnItemAdded(), Item is null."));
		return;
	}

	const UTetrisItemData* TetrisItemData = Cast<UTetrisItemData>(Item->GetItemData());
	UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);

	FIntPoint FoundSlot;
	bool bNeedsToRotate;
	if (!FindAvailableSlots(TetrisItem->GetRelativeSize(), FoundSlot, bNeedsToRotate))
		return;

	if (bNeedsToRotate)
		TetrisItem->Rotate();

	OccupySlotsWithItem(TetrisItem, FoundSlot); // Since we found the slots, occupy them with the item
}

void UTetrisInventory::OnItemRemoved_Implementation(UItemBase* Item)
{
	UTetrisItem* TetrisItem = Cast<UTetrisItem>(Item);

	const int32 CurrX = TetrisItem->GetCurrentPosition().X;
	const int32 CurrY = TetrisItem->GetCurrentPosition().Y;

	FreeSlots(TetrisItem->GetCurrentPosition(), TetrisItem->GetCachedSize());
	OnItemRemovedFromSlot.Broadcast(TetrisItem, TetrisItem->GetCurrentPosition());
}

void UTetrisInventory::OnClearedInventory_Implementation()
{
	for (int32 x = 0; x < GridSize.X; x++)
	{
		for (int32 y = 0; y < GridSize.Y; y++)
			InvMatrix[x][y]->FreeSlot();
	}
}

void UTetrisInventory::ConstructGrid()
{
	Capacity = GridSize.X * GridSize.Y;
	InvMatrix = TArray<TArray<UTetrisSlot*>>();
	InvMatrix.SetNum(GridSize.X);
	for (int32 x = 0; x < GridSize.X; x++)
	{
		InvMatrix[x].SetNum(GridSize.Y);
		for (int32 y = 0; y < GridSize.Y; y++)
		{
			InvMatrix[x][y] = NewObject<UTetrisSlot>(this);
			InvMatrix[x][y]->Init(this, FIntPoint(x, y));
		}
	}
}

bool UTetrisInventory::IsValidSlotPosition(const FIntPoint SlotPosition) const
{
	return InvMatrix.IsValidIndex(SlotPosition.X) && InvMatrix[SlotPosition.X].IsValidIndex(SlotPosition.Y);
}

void UTetrisInventory::FreeSlots(const FIntPoint StartPosition, const FIntPoint Size)
{
	for (int32 x = StartPosition.X; x < StartPosition.X + Size.X; x++)
	{
		for (int32 y = StartPosition.Y; y < StartPosition.Y + Size.Y; y++)
		{
			if (IsValidSlotPosition(FIntPoint(x, y))) // Safe check
				InvMatrix[x][y]->FreeSlot();
		}
	}
}

void UTetrisInventory::OccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList)
{
	const FIntPoint ItemSize = Item->GetRelativeSize();

	for (int32 x = StartSlotPosition.X; x < StartSlotPosition.X + ItemSize.X; x++)
	{
		for (int32 y = StartSlotPosition.Y; y < StartSlotPosition.Y + ItemSize.Y; y++)
			InvMatrix[x][y]->OccupySlot(Item);
	}

	Item->SetCurrentPosition(StartSlotPosition);
	Item->CacheCurrentRotation();

	if (bAddItemToList)
	{
		OnItemAddedToSlot.Broadcast(Item, Item->GetCurrentPosition());
		AddItemToList(Item);
	}
}

bool UTetrisInventory::TryOccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList)
{
	if (!CanOccupySlots(Item->GetRelativeSize(), StartSlotPosition))
		return false;

	OccupySlotsWithItem(Item, StartSlotPosition, bAddItemToList);
	return true;
}

bool UTetrisInventory::FindAvailableSlots(const FIntPoint Size, FIntPoint& OutStartSlotPosition, bool& OutbNeedsRotation) const
{
	auto TryFindSlot = [&](const FIntPoint& TestSize, const bool bRotationNeeded) -> bool
	{
		for (int32 x = 0; x <= GridSize.X - TestSize.X; x++)
		{
			for (int32 y = 0; y <= GridSize.Y - TestSize.Y; y++)
			{
				if (CanOccupySlots(TestSize, FIntPoint(x, y)))
				{
					OutStartSlotPosition = FIntPoint(x, y);
					OutbNeedsRotation = bRotationNeeded;
					return true;
				}
			}
		}

		return false;
	};

	// Try both orientations: normal and rotated
	return TryFindSlot(Size, false) || TryFindSlot(FIntPoint(Size.Y, Size.X), true);
}
