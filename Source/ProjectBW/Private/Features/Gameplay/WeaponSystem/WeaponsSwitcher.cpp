// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/WeaponsSwitcher.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/Factories/BWWeaponsFactory.h"

UWeaponsSwitcher::UWeaponsSwitcher(): WeaponEquipSlotKey(nullptr),
                                      Equipment(nullptr),
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
	Equipment->OnAnyItemEquipSlotChanged.AddDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipSlotChanged);
}

void UWeaponsSwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Equipment->OnAnyItemEquipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemEquipped);
	Equipment->OnAnyItemUnequipped.RemoveDynamic(this, &UWeaponsSwitcher::OnAnyItemUnequipped);
}

bool UWeaponsSwitcher::IsHoldingWeapon() const
{
	return HeldWeaponTuple.Key != nullptr || HeldWeaponTuple.Value != nullptr;
}

bool UWeaponsSwitcher::TryGetHeldWeapon(AWeaponBase*& OutWeapon) const
{
	if (!Check())
		return false;

	OutWeapon = HeldWeaponTuple.Key;
	return IsValid(OutWeapon);
}

void UWeaponsSwitcher::HoldWeaponAtSlot(const int32 SlotIndex)
{
	if (!Check() || !Weapons.Contains(SlotIndex))
		return;

	const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple = Weapons.FindRef(SlotIndex);
	if (WeaponTuple.Key == nullptr || HeldWeaponTuple.Key == WeaponTuple.Key)
		return;
	
	WithdrawWeapon(HeldWeaponTuple.Key);
	HoldWeapon(WeaponTuple);
}

void UWeaponsSwitcher::WithdrawWeaponAtSlot(int32 SlotIndex)
{
	if (!Check() || !Weapons.Contains(SlotIndex))
		return;

	const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple = Weapons.FindRef(SlotIndex);
	WithdrawWeapon(WeaponTuple.Key);
}

void UWeaponsSwitcher::AddWeaponActor(UBWWeaponItem* Item, const int32 SlotIndex)
{
	if (!Check() || !IsValid(Item))
		return;

	AWeaponBase* Weapon = UBWWeaponsFactory::CreateWeapon(OwnerPawn, Item);
	const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple = TTuple<AWeaponBase*, UBWWeaponItem*>(Weapon, Item);
	Weapons.Add(SlotIndex, WeaponTuple);
}

void UWeaponsSwitcher::RemoveWeaponActor(const int32 SlotIndex)
{
	if (!Check() || !Weapons.Contains(SlotIndex))
		return;

	const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple = Weapons.FindRef(SlotIndex);

	if (WeaponTuple == HeldWeaponTuple)
		HeldWeaponTuple = TTuple<AWeaponBase*, UBWWeaponItem*>(nullptr, nullptr);

	if (IsValid(WeaponTuple.Key))
		WeaponTuple.Key->Destroy();

	Weapons.Remove(SlotIndex);
}

bool UWeaponsSwitcher::IsWeapon(const UEquipSlotKey* EquipSlotKey, const UItemBase* Item) const
{
	return IsValid(Item) && EquipSlotKey == WeaponEquipSlotKey && Item->IsA(UBWWeaponItem::StaticClass());
}

void UWeaponsSwitcher::OnAnyItemEquipped(UEquipSlotKey* EquipSlotKey, int32 NewSlotIndex, UItemBase* Item)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnAnyItemEquipped"));

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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnAnyItemUnequipped"));
	RemoveWeaponActor(SlotIndex);
}

void UWeaponsSwitcher::OnAnyItemEquipSlotChanged(UEquipSlotKey* EquipSlotKey, UItemBase* Item, int32 NewSlotIndex, int32 OldSlotIndex)
{
	if (!IsWeapon(EquipSlotKey, Item))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnAnyItemEquipSlotChanged"));
	ChangeWeaponSlot(HeldWeaponTuple, NewSlotIndex);
}

void UWeaponsSwitcher::ChangeWeaponSlot(const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple, const int32 NewSlotIndex)
{
	if (!Check())
		return;

	if (Weapons.Contains(NewSlotIndex))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("WeaponsSwitcher::ChangeWeaponSlot: SlotIndex %d is already occupied"), NewSlotIndex);
		return;
	}

	const int* KeyRef = Weapons.FindKey(WeaponTuple);
	if (KeyRef == nullptr)
	{
		UE_LOG(LogBWWeapons, Error, TEXT("WeaponsSwitcher::ChangeWeaponSlot: Weapon is not in the map"));
		return;
	}

	const int32 OldSlotIndex = *KeyRef;
	Weapons.Add(NewSlotIndex, WeaponTuple);
	Weapons.Remove(OldSlotIndex);
}

void UWeaponsSwitcher::HoldWeapon(const TTuple<AWeaponBase*, UBWWeaponItem*> WeaponTuple)
{
	if (!Check() || WeaponTuple.Key == nullptr || WeaponTuple.Value == nullptr)
		return;

	HeldWeaponTuple = WeaponTuple;
	ShowWeaponActor(HeldWeaponTuple.Key);
	HeldWeaponTuple.Key->AttachToComponent(AttachToComponentEquipped, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedWeaponAttachSocketName);
	OnWeaponHeld.Broadcast(HeldWeaponTuple.Key);
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
