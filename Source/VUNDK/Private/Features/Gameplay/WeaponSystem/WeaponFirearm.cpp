// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"

AWeaponFirearm::AWeaponFirearm()
{
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("Shooter"));
}

void AWeaponFirearm::SetWeaponShootType(const EShootType NewShootType)
{
	WeaponShootType = NewShootType;
}

int32 AWeaponFirearm::Reload(const int32 Ammo)
{
	const int32 Remain = Shooter->Refill(Ammo);
	OnReload();
	return Remain;
}

void AWeaponFirearm::ReloadAllMagazine()
{
	Shooter->RefillAllMagazine();
	OnReload();
}

void AWeaponFirearm::BeginPlay()
{
	Super::BeginPlay();
	Shooter->SetShooterDamage(WeaponData.Damage);
}

void AWeaponFirearm::OnReload_Implementation()
{
}

bool AWeaponFirearm::OnWeaponAttack_Implementation()
{
	Super::OnWeaponAttack_Implementation();
	return Shooter->Shoot(WeaponShootType);
}
