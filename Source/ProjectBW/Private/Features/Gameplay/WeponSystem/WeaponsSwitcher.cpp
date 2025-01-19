// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/WeponSystem/WeaponsSwitcher.h"
#include "Features/Gameplay/WeponSystem/Factories/BWWeaponsFactory.h"

UWeaponsSwitcher::UWeaponsSwitcher(): WeaponEquipSlotKey(nullptr),
                                      Equipment(nullptr),
                                      CurrentEquippedWeapon(nullptr),
                                      OwnerPawn(nullptr),
                                      AttachToComponentUnequipped(nullptr),
                                      AttachToComponentEquipped(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponsSwitcher::Init(UEquipment* InEquipment, USceneComponent* InAttachToComponentEquipped, FName InEquipppedWeaponAttachSocketName, USceneComponent* InAttachToComponentUnequipped, FName InUnequipppedWeaponAttachSocketName)
{
	AttachToComponentEquipped = InAttachToComponentEquipped;
	AttachToComponentUnequipped = InAttachToComponentUnequipped;
	EquippedWeaponAttachSocketName = InEquipppedWeaponAttachSocketName;
	UnequippedWeaponAttachSocketName = InUnequipppedWeaponAttachSocketName;
	OwnerPawn = Cast<APawn>(GetOwner());
	Equipment = InEquipment;
	
	Equipment->OnAnyItemEquipped.AddDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipped.AddDynamic(this, &UWeaponsSwitcher::OnAnyItemUnequipped);
}

void UWeaponsSwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Equipment->OnAnyItemEquipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemUnequipped);
}

bool UWeaponsSwitcher::TryGetEquippedWeapon(AWeaponBase*& OutWeapon) const
{
	if (!Check())
		return false;

	OutWeapon = CurrentEquippedWeapon;
	return IsValid(OutWeapon);
}

bool UWeaponsSwitcher::TryEquipWeaponAtSlot(const int32 SlotIndex)
{
	if (!Check())
		return false;

	AWeaponBase* WeaponToEquip = Weapons.FindRef(SlotIndex);

	if (!IsValid(WeaponToEquip))
	{
		UE_LOG(LogWeaponsSwitcher, Error, TEXT("WeaponsSwitcher::TryEquipWeaponAtSlot: WeaponToEquip is nullptr"));
		return false;
	}

	if (CurrentEquippedWeapon == WeaponToEquip)
		return true;
	
	TryUnequipCurrentWeapon();
	CurrentEquippedWeapon = WeaponToEquip;
	ShowWeaponActor(CurrentEquippedWeapon);
	CurrentEquippedWeapon->AttachToComponent(AttachToComponentEquipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedWeaponAttachSocketName);
	OnNewWeaponEquipped.Broadcast(CurrentEquippedWeapon);
	return true;
}

bool UWeaponsSwitcher::TryUnequipCurrentWeapon()
{
	if (!Check() || !IsValid(CurrentEquippedWeapon))
		return false;

	if (IsValid(AttachToComponentUnequipped))
		CurrentEquippedWeapon->AttachToComponent(AttachToComponentUnequipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnequippedWeaponAttachSocketName);
	else
		HideWeaponActor(CurrentEquippedWeapon);

	CurrentEquippedWeapon = nullptr;
	OnWeaponUnequipped.Broadcast();
	return true;
}

void UWeaponsSwitcher::AddWeaponActor(UWeaponItem* Item, const int32 SlotIndex)
{
	if (!Check() || !IsValid(Item))
		return;

	AWeaponBase* Weapon = UBWWeaponsFactory::CreateWeapon(OwnerPawn, Item);
	if (!IsValid(Weapon))
		return;

	HideWeaponActor(Weapon);
	Weapons.Add(SlotIndex, Weapon);

	if (bEquipWeaponOnFirstSlot && SlotIndex == 0)
		TryEquipWeaponAtSlot(SlotIndex);
}

void UWeaponsSwitcher::RemoveWeaponActor(const int32 SlotIndex)
{
	if (!Check() || !Weapons.Contains(SlotIndex))
		return;

	if (AWeaponBase* Weapon = Weapons.FindRef(SlotIndex); IsValid(Weapon))
		Weapon->Destroy();
	
	Weapons.Remove(SlotIndex);
}

bool UWeaponsSwitcher::IsWeapon(const UEquipSlotKey* EquipSlotKey, const UItemBase* Item) const
{
	return IsValid(Item) && EquipSlotKey == WeaponEquipSlotKey && Item->IsA(UWeaponItem::StaticClass());
}

void UWeaponsSwitcher::OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	UWeaponItem* WeaponItem = Cast<UWeaponItem>(Item);
	if (!IsValid(WeaponItem))
	{
		UE_LOG(LogWeaponsSwitcher, Error, TEXT("WeaponSwitccher::OnAnyItemEquipped: WeaponItem is nullptr"));
		return;
	}

	AddWeaponActor(WeaponItem, SlotIndex);
}

void UWeaponsSwitcher::OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	RemoveWeaponActor(SlotIndex);
}

void UWeaponsSwitcher::ShowWeaponActor(AWeaponBase* Weapon)
{
	if (!IsValid(Weapon))
		return;
	
	Weapon->SetActorHiddenInGame(false);
}

void UWeaponsSwitcher::HideWeaponActor(AWeaponBase* Weapon)
{
	if (!IsValid(Weapon))
		return;
	
	Weapon->SetActorHiddenInGame(true);
}

bool UWeaponsSwitcher::Check() const
{
	return IsValid(Equipment) && IsValid(WeaponEquipSlotKey) && IsValid(OwnerPawn) && IsValid(AttachToComponentEquipped);
}
