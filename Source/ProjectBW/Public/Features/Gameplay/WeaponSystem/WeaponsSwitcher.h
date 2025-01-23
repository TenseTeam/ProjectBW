// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Gameplay/EquipmentSystem/Equipment.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponBase.h"
#include "WeaponsSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnNewWeaponEquipped,
	AWeaponBase*, Weapon
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponUnequipped
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTBW_API UWeaponsSwitcher : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnNewWeaponEquipped OnNewWeaponEquipped;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponUnequipped OnWeaponUnequipped;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquipSlotKey* WeaponEquipSlotKey;

private:
	UPROPERTY()
	UEquipment* Equipment;
	UPROPERTY()
	TMap<int32, AWeaponBase*> Weapons;
	UPROPERTY()
	AWeaponBase* CurrentEquippedWeapon;
	UPROPERTY()
	APawn* OwnerPawn;
	UPROPERTY()
	USceneComponent* AttachToComponentUnequipped;
	UPROPERTY()
	USceneComponent* AttachToComponentEquipped;
	FName EquippedWeaponAttachSocketName;
	FName UnequippedWeaponAttachSocketName;

public:
	UWeaponsSwitcher();
	
	UFUNCTION(BlueprintCallable)
	void Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedWeaponAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedWeaponAttachSocketName);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	bool IsAnyWeaponEquipped() const;
	
	UFUNCTION(BlueprintPure)
	bool TryGetEquippedWeapon(AWeaponBase*& OutWeapon) const;

	UFUNCTION(BlueprintCallable)
	bool TryEquipWeaponAtSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool TryUnequipCurrentWeapon();

protected:
	void AddWeaponActor(UBWWeaponItem* Item, int32 SlotIndex);

	void RemoveWeaponActor(int32 SlotIndex);

private:
	UFUNCTION()
	void OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item);
	
	UFUNCTION()
	void OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item);

	void WithdrawWeapon(AWeaponBase* Weapon) const;
	
	void AttachWeaponToUnequipSocket(AWeaponBase* Weapon) const;
	
	static void ShowWeaponActor(AWeaponBase* Weapon);

	static void HideWeaponActor(AWeaponBase* Weapon);

	bool IsWeapon(const UEquipSlotKey* EquipSlotKey, const UItemBase* Item) const;

	bool Check() const;
};
