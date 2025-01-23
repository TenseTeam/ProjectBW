// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"

ABWWeaponFirearm::ABWWeaponFirearm(): WeaponFirearmItem(nullptr)
{
}

void ABWWeaponFirearm::InitBWWeapon(APawn* InOwner, UBWWeaponFirearmItem* InWeaponItem)
{
	Super::Init(InOwner);
	WeaponFirearmItem = InWeaponItem;

	SetWeaponDamage(WeaponFirearmItem->GetWeaponDamage());
	SetWeaponFireRate(WeaponFirearmItem->GetWeaponItemFireRate());
	SetWeaponMaxRange(WeaponFirearmItem->GetWeaponItemMaxRange());

	const int32 MagSize = WeaponFirearmItem->GetWeaponItemMagSize();
	SetWeaponMagSize(MagSize);
	WeaponFirearmItem->TryConsumeWeaponItemNewMagazine(MagSize);
	SetWeaponAmmoRemaining(WeaponFirearmItem->GetWeaponItemAmmoRemaining());
}

void ABWWeaponFirearm::OnReload_Implementation()
{
	Super::OnReload_Implementation();
	WeaponFirearmItem->SetWeaponItemAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
}

bool ABWWeaponFirearm::DeployWeaponAttack_Implementation()
{
	WeaponFirearmItem->SetWeaponItemAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
	return Super::DeployWeaponAttack_Implementation();
}
