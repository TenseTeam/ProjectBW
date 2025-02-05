// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EquipSlotKey.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Equipment.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogEquipment, All, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemEquipped,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyItemUnequipped,
	UEquipSlotKey*, EquipSlotKey,
	int32, SlotIndex,
	UItemBase*, Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnAnyItemEquipSlotChanged,
	UEquipSlotKey*, EquipSlotKey,
	UItemBase*, Item,
	int32, NewSlotIndex,
	int32, OldSlotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEquipChanged
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEquipmentCleared
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UEquipment : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemEquipped OnAnyItemEquipped;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemUnequipped OnAnyItemUnequipped;
	UPROPERTY(BlueprintAssignable)
	FOnAnyItemEquipSlotChanged OnAnyItemEquipSlotChanged;
	UPROPERTY(BlueprintAssignable)
	FOnEquipChanged OnEquipChanged;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentCleared OnEquipmentCleared;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<UEquipSlotKey*, int32> SlotLimits;

private:
	TMap<FName, TMap<int32, UItemBase*>> EquipSlots;

public:
	UEquipment();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UItemBase* Item, UEquipSlotKey* TargetSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory = true);

	UFUNCTION(BlueprintCallable)
	bool TryUnequipItem(UItemBase* Item);
	
	UFUNCTION(BlueprintCallable)
	TSet<UItemBase*> GetEquippedItems();

	UFUNCTION(BlueprintCallable)
	void ClearEquipment();

protected:
	virtual bool CanEquipItem(const UItemBase* Item, const UEquipSlotKey* TargetSlotKey, const int32 SlotIndex) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEquipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequipItem(UEquipSlotKey* EquipSlotKey, UItemBase* Item);
	
private:
	void EquipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 SlotIndex, const bool bRemoveFromInventory = true);

	void UnequipItem(UItemBase* Item, UEquipSlotKey* EquipSlotKey);
	
	void AddItemInEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex);

	void RemoveItemFromEquipSlot(UItemBase* Item, const UEquipSlotKey* EquipSlotKey, const int32 SlotIndex);

	void ChangeItemEquipSlot(UItemBase* Item, UEquipSlotKey* EquipSlotKey, const int32 NewSlotIndex);
	
	UFUNCTION()
	void OnItemAddedToAnyInventory(UItemBase* Item, UInventoryBase* Inventory);
};
