// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SaveData/InventoryBaseSaveData.h"
#include "Features/Gameplay/EquipmentSystem/Equipment.h"
#include "Features/Generic/SaveSystem/Interfaces/Saveable.h"
#include "InventoryBase.generated.h"

struct FItemSaveData;
class UItemsRegistry;
class UItemBase;
class UItemDataBase;

DEFINE_LOG_CATEGORY_STATIC(LogInventorySystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemAdded,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemConsumed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemUsed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemRemoved,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryCleared
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryModified
);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup=(Custom))
class VUNDK_API UInventoryBase : public UActorComponent, public ISaveable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryCleared OnInventoryCleared;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryModified OnInventoryModified;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemAdded OnAnyItemAdded;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemUsed OnAnyItemUsed;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemConsumed OnAnyItemConsumed;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemRemoved OnAnyItemRemoved;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1", ToolTip = "The maximum number of items that can be stored in the inventory."))
	int32 Capacity = 16;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<UItemBase*> Items;

private:
	UPROPERTY()
	UEquipment* RelatedEquipment;

public:
	UInventoryBase();
	
	UFUNCTION(BlueprintPure)
	virtual USaveData* CreateSaveData() override;

	UFUNCTION(BlueprintCallable)
	bool LoadSaveData(USaveData* SavedData) override;

	UFUNCTION(BlueprintCallable)
	void SetRelatedEquipment(UEquipment* Equipment);
	
	UFUNCTION(BlueprintCallable)
	UItemBase* AddNewItem(UItemDataBase* ItemData);

	UFUNCTION(BlueprintCallable)
	void AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemOfType(UItemDataBase* ItemData);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	void ClearInventory();

	UFUNCTION(BlueprintCallable)
	UItemBase* Find(const UItemDataBase* ItemData) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsFull() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsEmpty() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanContainItem(const UItemDataBase* ItemData) const;

	UFUNCTION(BlueprintPure)
	bool HasItemOfTypeID(const UItemDataBase* ItemData) const;

	UFUNCTION(BlueprintCallable)
	UItemDataBase* GetItemDataByID(const FName ItemDataID) const;

protected:
	TSet<UItemBase*> GetAllItems();
	
	void AddItemToList(UItemBase* Item);

	void RemoveItemFromList(UItemBase* Item);

	UFUNCTION(BlueprintNativeEvent)
	void LoadInventorySaveData(UInventoryBaseSaveData* InventorySaveData);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnItemAdded(UItemBase* Item);

	UFUNCTION(BlueprintNativeEvent)
	void OnItemRemoved(UItemBase* Item);

	UFUNCTION(BlueprintNativeEvent)
	void OnClearedInventory();
};
