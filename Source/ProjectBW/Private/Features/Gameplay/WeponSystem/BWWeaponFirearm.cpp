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

bool ABWWeaponFirearm::DeployWeaponAttack_Implementation()
{
	WeaponFirearmItem->SetAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
	return Super::DeployWeaponAttack_Implementation();
}
