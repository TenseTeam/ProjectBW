// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Gameplay/EquipmentSystem/Equipment.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponBase.h"
#include "WeaponsSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWeaponHeld,
	AWeaponBase*, Weapon
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnWeaponWithdraw,
	AWeaponBase*, Weapon
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTBW_API UWeaponsSwitcher : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponHeld OnWeaponHeld;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponWithdraw OnWeaponWithdraw;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquipSlotKey* WeaponEquipSlotKey;

private:
	UPROPERTY()
	UEquipment* Equipment;
	UPROPERTY()
	APawn* OwnerPawn;
	UPROPERTY()
	USceneComponent* AttachToComponentUnequipped;
	UPROPERTY()
	USceneComponent* AttachToComponentEquipped;
	UPROPERTY()
	AWeaponBase* HeldWeapon;
	UPROPERTY()
	TMap<int32, AWeaponBase*> CurrentWeapons;
	FName EquippedWeaponAttachSocketName;
	FName UnequippedWeaponAttachSocketName;
	int32 CurrentWeaponSlotIndex;
	TArray<int32> CurrentWeaponKeys;
	
public:
	UWeaponsSwitcher();
	
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedWeaponAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedWeaponAttachSocketName);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	bool IsHoldingWeapon() const;
	
	UFUNCTION(BlueprintPure)
	bool TryGetHeldWeapon(AWeaponBase*& OutWeapon) const;

	UFUNCTION(BlueprintCallable)
	void HoldNextWeapon();

	UFUNCTION(BlueprintCallable)
	void HoldPreviousWeapon();

	UFUNCTION(BlueprintCallable)
	void HoldWeaponAtSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void WithdrawWeaponAtSlot(int32 SlotIndex) const;

protected:
	void AddWeaponActor(UBWWeaponItem* Item, int32 SlotIndex);

	void RemoveWeaponActor(int32 SlotIndex);

private:
	UFUNCTION()
	void OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex, UItemBase* Item);
	
	UFUNCTION()
	void OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item);

	UFUNCTION()
	void OnAnyItemEquipSlotChanged(UEquipSlotKey* EquipSlotKey, UItemBase* Item, int32 NewSlotIndex, int32 OldSlotIndex);

	void ChangeWeaponSlot(AWeaponBase* Weapon, const int32 NewSlotIndex);

	void HoldWeapon(AWeaponBase* Weapon);
	
	void WithdrawWeapon(AWeaponBase* Weapon) const;
	
	static void ShowWeaponActor(AWeaponBase* Weapon);

	static void HideWeaponActor(AWeaponBase* Weapon);

	bool IsWeapon(const UEquipSlotKey* EquipSlotKey, const UItemBase* Item) const;

	static bool HasSameWeaponItem(const AWeaponBase* WeaponA, const AWeaponBase* WeaponB);

	void UpdateWeaponKeys();
	
	bool Check() const;
};
