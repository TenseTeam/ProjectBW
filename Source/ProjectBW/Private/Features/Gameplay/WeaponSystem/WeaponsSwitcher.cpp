// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/WeaponsSwitcher.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/Factories/BWWeaponsFactory.h"

UWeaponsSwitcher::UWeaponsSwitcher(): WeaponEquipSlotKey(nullptr),
                                      Equipment(nullptr),
                                      OwnerPawn(nullptr),
                                      AttachToComponentUnequipped(nullptr),
                                      AttachToComponentEquipped(nullptr),
                                      HeldWeapon(nullptr),
                                      CurrentWeaponSlotIndex(0)
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
	Equipment->OnAnyItemEquipSlotChanged.AddDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipSlotChanged);
}

void UWeaponsSwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Equipment->OnAnyItemEquipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemUnequipped);
	Equipment->OnAnyItemEquipSlotChanged.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipSlotChanged);
}

bool UWeaponsSwitcher::IsHoldingWeapon() const
{
	return IsValid(HeldWeapon);
}

bool UWeaponsSwitcher::TryGetHeldWeapon(AWeaponBase*& OutWeapon) const
{
	if (!Check())
		return false;

	OutWeapon = HeldWeapon;
	return IsValid(OutWeapon);
}

void UWeaponsSwitcher::HoldNextWeapon()
{
	if (!Check() || CurrentWeapons.Num() <= 0)
		return;

	int32 NextIndex = CurrentWeaponSlotIndex + 1;
	if (NextIndex >= CurrentWeapons.Num())
		NextIndex = 0;
	
	const int32 SlotIndex = CurrentWeaponKeys[NextIndex];
	HoldWeaponAtSlot(SlotIndex);
	CurrentWeaponSlotIndex = SlotIndex;
}

void UWeaponsSwitcher::HoldPreviousWeapon()
{
	if (!Check() || CurrentWeapons.Num() <= 0)
		return;

	int32 PreviousIndex = CurrentWeaponSlotIndex - 1;
	if (PreviousIndex < 0)
		PreviousIndex = CurrentWeapons.Num() - 1;
	
	const int32 SlotIndex = CurrentWeaponKeys[PreviousIndex];
	HoldWeaponAtSlot(SlotIndex);
	CurrentWeaponSlotIndex = SlotIndex;
}

void UWeaponsSwitcher::HoldWeaponAtSlot(const int32 SlotIndex)
{
	if (!Check() || !CurrentWeapons.Contains(SlotIndex))
		return;

	AWeaponBase* Weapon = CurrentWeapons.FindRef(SlotIndex);
	if (!IsValid(Weapon) || HasSameWeaponItem(HeldWeapon, Weapon))
		return;

	WithdrawWeapon(HeldWeapon);
	HoldWeapon(Weapon);
}

void UWeaponsSwitcher::WithdrawWeaponAtSlot(const int32 SlotIndex) const
{
	if (!Check() || !CurrentWeapons.Contains(SlotIndex))
		return;

	AWeaponBase* Weapon = CurrentWeapons.FindRef(SlotIndex);
	WithdrawWeapon(Weapon);
}

void UWeaponsSwitcher::AddWeaponActor(UBWWeaponItem* Item, const int32 SlotIndex)
{
	if (!Check() || !IsValid(Item))
		return;

	AWeaponBase* Weapon = UBWWeaponsFactory::CreateWeapon(OwnerPawn, Item);
	CurrentWeapons.Add(SlotIndex, Weapon);
	UpdateWeaponKeys();
}

void UWeaponsSwitcher::RemoveWeaponActor(const int32 SlotIndex)
{
	if (!Check() || !CurrentWeapons.Contains(SlotIndex))
		return;

	AWeaponBase* Weapon = CurrentWeapons.FindRef(SlotIndex);

	if (Weapon == HeldWeapon)
	{
		CurrentWeaponSlotIndex = 0;
		HeldWeapon = nullptr;
	}

	if (IsValid(Weapon))
		Weapon->Destroy();

	CurrentWeapons.Remove(SlotIndex);
	UpdateWeaponKeys();
}

bool UWeaponsSwitcher::IsWeapon(const UEquipSlotKey* EquipSlotKey, const UItemBase* Item) const
{
	return IsValid(Item) && EquipSlotKey == WeaponEquipSlotKey && Item->IsA(UBWWeaponItem::StaticClass());
}

bool UWeaponsSwitcher::HasSameWeaponItem(const AWeaponBase* WeaponA, const AWeaponBase* WeaponB)
{
	if (!IsValid(WeaponA) || !IsValid(WeaponB))
		return false;

	return WeaponA->GetPayload() == WeaponB->GetPayload();
}

void UWeaponsSwitcher::UpdateWeaponKeys()
{
	CurrentWeapons.GetKeys(CurrentWeaponKeys);
}

void UWeaponsSwitcher::OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex, UItemBase* Item)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	UBWWeaponItem* WeaponItem = Cast<UBWWeaponItem>(Item);
	if (!IsValid(WeaponItem))
		return;

	AddWeaponActor(WeaponItem, NewSlotIndex);

	if (!IsHoldingWeapon())
		HoldWeaponAtSlot(NewSlotIndex);
	else
		WithdrawWeaponAtSlot(NewSlotIndex);
}

void UWeaponsSwitcher::OnAnyItemUnequipped(UEquipSlotKey* EquipSlotKey, int32 SlotIndex, UItemBase* Item)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	RemoveWeaponActor(SlotIndex);
}

void UWeaponsSwitcher::OnAnyItemEquipSlotChanged(UEquipSlotKey* EquipSlotKey, UItemBase* Item, int32 NewSlotIndex, int32 OldSlotIndex)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	ChangeWeaponSlot(HeldWeapon, NewSlotIndex);
}

void UWeaponsSwitcher::ChangeWeaponSlot(AWeaponBase* Weapon, const int32 NewSlotIndex)
{
	if (!Check())
		return;

	if (CurrentWeapons.Contains(NewSlotIndex))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("WeaponsSwitcher::ChangeWeaponSlot: SlotIndex %d is already occupied"), NewSlotIndex);
		return;
	}

	const int* KeyRef = CurrentWeapons.FindKey(Weapon);
	if (KeyRef == nullptr)
	{
		UE_LOG(LogBWWeapons, Error, TEXT("WeaponsSwitcher::ChangeWeaponSlot: Weapon is not in the map"));
		return;
	}

	const int32 OldSlotIndex = *KeyRef;
	CurrentWeapons.Add(NewSlotIndex, Weapon);
	CurrentWeapons.Remove(OldSlotIndex);
	UpdateWeaponKeys();
}

void UWeaponsSwitcher::HoldWeapon(AWeaponBase* Weapon)
{
	if (!Check() || !IsValid(Weapon))
		return;

	HeldWeapon = Weapon;
	ShowWeaponActor(HeldWeapon);
	HeldWeapon->AttachToComponent(AttachToComponentEquipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedWeaponAttachSocketName);
	OnWeaponHeld.Broadcast(HeldWeapon);
}

void UWeaponsSwitcher::WithdrawWeapon(AWeaponBase* Weapon) const
{
	if (!Check() || !IsValid(Weapon))
		return;

	if (IsValid(AttachToComponentUnequipped))
		Weapon->AttachToComponent(AttachToComponentUnequipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, UnequippedWeaponAttachSocketName);
	else
		HideWeaponActor(Weapon);

	OnWeaponWithdraw.Broadcast(Weapon);
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
