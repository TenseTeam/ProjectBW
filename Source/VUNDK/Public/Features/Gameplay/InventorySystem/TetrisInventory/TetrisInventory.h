// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisSlot.h"
#include "Data/SaveData/TetrisItemSaveData.h"
#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "TetrisInventory.generated.h"

class UTetrisItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemAddedToSlot,
	UTetrisItem*, Item,
	FIntPoint, SlotPosition
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemRemovedFromSlot,
	UTetrisItem*, Item,
	FIntPoint, SlotPosition
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnItemMoved,
	UTetrisItem*, Item,
	FIntPoint, OldPosition,
	FIntPoint, NewPosition
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("InventoryBase"))
class VUNDK_API UTetrisInventory : public UInventoryBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemAddedToSlot OnItemAddedToSlot;
	UPROPERTY(BlueprintAssignable)
	FOnItemRemovedFromSlot OnItemRemovedFromSlot;
	UPROPERTY(BlueprintAssignable)
	FOnItemMoved OnItemMoved;

	UPROPERTY(EditDefaultsOnly)
	FIntPoint GridSize;

private:
	TArray<TArray<UTetrisSlot*>> InvMatrix;

public:
	UTetrisInventory();

	virtual USaveData* CreateSaveData() override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	virtual bool CanContainItem(const UItemDataBase* ItemData) const override;

	virtual bool IsFull() const override;

	UFUNCTION(BlueprintPure)
	UTetrisSlot* GetSlot(const FIntPoint SlotPosition) const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetGridSize() const;

	UFUNCTION(BlueprintPure)
	bool CanOccupySlots(const FIntPoint Size, const FIntPoint StartSlotPosition, const UTetrisItem* IgnoreItem = nullptr) const;

	UFUNCTION(BlueprintCallable)
	bool TryAddNewItemAtSlots(UTetrisItemData* ItemData, const FIntPoint StartSlotPosition);

	UFUNCTION(BlueprintCallable)
	bool TryAddItemAtSlots(UTetrisItem* Item, const FIntPoint StartSlotPosition);

	UFUNCTION(BlueprintCallable)
	bool TryMoveItem(UTetrisItem* Item, const FIntPoint NewPosition);

protected:
	virtual void BeginPlay() override;
	
	virtual void OnItemAdded_Implementation(UItemBase* Item) override;

	virtual void OnItemRemoved_Implementation(UItemBase* Item) override;

	virtual void OnClearedInventory_Implementation() override;

private:
	void ConstructGrid();

	bool IsValidSlotPosition(const FIntPoint SlotPosition) const;

	void FreeSlots(const FIntPoint StartPosition, const FIntPoint Size);

	void OccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList = true);

	bool TryOccupySlotsWithItem(UTetrisItem* Item, const FIntPoint StartSlotPosition, const bool bAddItemToList = true);

	bool FindAvailableSlots(const FIntPoint Size, FIntPoint& OutStartSlotPosition, bool& OutbNeedsRotation) const;
};
