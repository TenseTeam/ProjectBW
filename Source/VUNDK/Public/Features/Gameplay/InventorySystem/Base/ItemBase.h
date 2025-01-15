// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/ItemSaveData.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

class UClassicInventory;
class UItemDataBase;
class UEquipment;
class AItemDropActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemAdded,
	UItemBase*, Item,
	UInventoryBase*, Inventory
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemUsed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemConsumed,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemRemoved,
	UItemBase*, Item,
	UInventoryBase*, Inventory
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemEquipped,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemUnequipped,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnItemQuantityChanged,
	UItemBase*, Item,
	int32, Quantity
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API UItemBase : public UObject
{
	GENERATED_BODY()

	friend class UInventoryBase;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;
	UPROPERTY(BlueprintAssignable)
	FOnItemUsed OnItemUsed;
	UPROPERTY(BlueprintAssignable)
	FOnItemConsumed OnItemConsumed;
	UPROPERTY(BlueprintAssignable)
	FOnItemRemoved OnItemRemoved;
	UPROPERTY(BlueprintAssignable)
	FOnItemEquipped OnItemEquipped;
	UPROPERTY(BlueprintAssignable)
	FOnItemUnequipped OnItemUnequipped;
	UPROPERTY(BlueprintAssignable)
	FOnItemQuantityChanged OnItemQuantityChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryBase* RelatedInventory;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItemDataBase* ItemData;

private:
	UPROPERTY()
	UEquipment* RelatedEquipment;
	UPROPERTY()
	int32 EquipSlotIndex;
	UPROPERTY()
	int32 CurrentQuantity;

public:
	UItemBase();

	FItemSaveData CreateItemBaseSaveData() const;

	void LoadItemBaseSaveData(UInventoryBase* LoadingInventory, const FItemSaveData ItemSaveData, bool& bOutHasBeenEquipped);

	virtual void Init(UItemDataBase* Data);

	UFUNCTION(BlueprintPure)
	UItemDataBase* GetItemData() const;

	void AssignInventory(UInventoryBase* Inventory);

	void DeassignInventory();

	void SetEquipSlot(UEquipment* InEquipment, const int32 SlotIndex);

	void ClearEquipSlot();

	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	UFUNCTION(BlueprintCallable)
	bool TryDrop(APlayerController* PlayerController, FVector Location, FRotator Rotation);
	
	UFUNCTION(BlueprintCallable)
	void Remove();
	
	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable)
	int32 Consume(const int32 AmountToConsume = 1);

	UFUNCTION(BlueprintPure)
	virtual FText GetItemFullName() const;

	UFUNCTION(BlueprintPure)
	virtual FSlateBrush GetItemIcon() const;

	UFUNCTION(BlueprintPure)
	virtual FText GetItemDescription() const;

	UFUNCTION(BlueprintPure)
	virtual UStaticMesh* GetItemMesh() const;
	
	int32 GetEquipSlotIndex() const;

	UFUNCTION(BlueprintPure)
	bool CanStack() const;

	UFUNCTION(BlueprintNativeEvent)
	bool CanStackItem(UItemBase* OtherItem) const;
	
	UFUNCTION(BlueprintPure)
	bool IsMaxStacked() const;
	
	UFUNCTION(BlueprintPure)
	bool IsStackable() const;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentQuantity() const;

	UFUNCTION(BlueprintCallable)
	bool TryStackItem(UItemBase* Item, const int32 AmountToStack = 1);

	UFUNCTION(BlueprintCallable)
	bool TrySplitItem(const int32 AmountToSplit, UItemBase*& OutNewSplittedItem);
	
	int32 IncreaseQuantity(int32 Amount = 1);

	int32 DecreaseQuantity(const int32 Amount = 1);

	int32 SetQuantity(const int32 Quantity);

	UItemBase* DuplicateItem() const;
	
protected:
	virtual void OnPreInit(UItemDataBase* Data);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnAdd();

	UFUNCTION(BlueprintNativeEvent)
	void OnRemove();

	UFUNCTION(BlueprintNativeEvent)
	void OnEquip();

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnUse();

	UFUNCTION(BlueprintNativeEvent)
	void OnConsume();

	UFUNCTION(BlueprintPure)
	AGameModeBase* GetGameMode() const;

	UFUNCTION(BlueprintPure)
	APlayerController* GetPlayerController(const int PlayerIndex) const;

	UFUNCTION(BlueprintPure)
	APlayerState* GetPlayerState(const int PlayerStateIndex) const;

private:
	bool CanDrop() const;
};


