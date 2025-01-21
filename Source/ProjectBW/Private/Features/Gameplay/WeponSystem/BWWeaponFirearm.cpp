// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/WeponSystem/BWWeaponFirearm.h"

ABWWeaponFirearm::ABWWeaponFirearm(): WeaponFirearmItem(nullptr)
{
}

void ABWWeaponFirearm::SetWeaponFirearmItem(UWeaponFirearmItem* NewWeaponFirearmItem)
{
	WeaponFirearmItem = NewWeaponFirearmItem;
}

void ABWWeaponFirearm::OnReload_Implementation()
{
	Super::OnReload_Implementation();
	WeaponFirearmItem->SetAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
}

bool ABWWeaponFirearm::OnWeaponAttack_Implementation()
{
	WeaponFirearmItem->SetAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
	return Super::OnWeaponAttack_Implementation();
}
