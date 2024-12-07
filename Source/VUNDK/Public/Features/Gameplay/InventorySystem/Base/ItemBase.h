// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/ItemSaveData.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

class UClassicInventory;
class UItemDataBase;
class UEquipment;

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryBase* RelatedInventory;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItemDataBase* ItemData;

private:
	int32 EquipSlotIndex;

public:
	UItemBase();

	FItemSaveData CreateItemBaseSaveData() const;

	void LoadItemBaseSaveData(const FItemSaveData ItemSaveData);

	virtual void Init(UItemDataBase* Data);

	UFUNCTION(BlueprintPure)
	UItemDataBase* GetItemData() const;

	void AssignInventory(UInventoryBase* Inventory);

	void DeassignInventory();

	void Equip(const int32 SlotIndex);

	void Unequip();
	
	UFUNCTION(BlueprintCallable)
	void Remove();
	
	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable)
	void Consume();

	UFUNCTION(BlueprintPure)
	virtual FText GetItemFullName() const;

	UFUNCTION(BlueprintPure)
	virtual FSlateBrush GetItemIcon() const;

	UFUNCTION(BlueprintPure)
	virtual FText GetItemDescription() const;

	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	int32 GetEquipSlotIndex() const;

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
};
