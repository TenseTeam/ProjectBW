// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"

AWeaponFirearm::AWeaponFirearm()
{
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("Shooter"));
	ShootBarrel = CreateDefaultSubobject<UShootBarrel>(TEXT("ShootBarrel"));
}

void AWeaponFirearm::PostInitProperties()
{
	Super::PostInitProperties();

	if (WeaponMesh->DoesSocketExist(ShootBarrelSocketName))
		ShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShootBarrelSocketName);
	else
		ShootBarrel->SetupAttachment(WeaponMesh);
}

void AWeaponFirearm::Init(APawn* InOwner)
{
	Super::Init(InOwner);
	Shooter->SetOwner(InOwner);
}

void AWeaponFirearm::ResetRecoil() const
{
	Shooter->ResetRecoil();
}

void AWeaponFirearm::SetWeaponDamage(const float NewDamage)
{
	Super::SetWeaponDamage(NewDamage);
	Shooter->ShooterBehaviour->SetDamage(NewDamage);
}

void AWeaponFirearm::SetWeaponFireRate(const float NewFireRate) const
{
	Shooter->ShooterBehaviour->SetFireRate(NewFireRate);
}

void AWeaponFirearm::SetWeaponMagSize(const int32 NewMagSize) const
{
	Shooter->ShooterBehaviour->SetMagSize(NewMagSize);
}

void AWeaponFirearm::SetWeaponMaxRange(const float NewRange) const
{
	Shooter->ShooterBehaviour->SetMaxRange(NewRange);
}

void AWeaponFirearm::SetWeaponAmmoRemaining(const int32 NewAmmoRemaining) const
{
	Shooter->ShooterBehaviour->SetCurrentAmmo(NewAmmoRemaining);
}

void AWeaponFirearm::SetWeaponShootType(const EShootType NewShootType)
{
	WeaponShootType = NewShootType;
}

void AWeaponFirearm::SetWeaponMaxSpread(const float NewSpread) const
{
	Shooter->ShooterBehaviour->SetMaxSpread(NewSpread);
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
	Shooter->Init(ShootBarrel);
	SetWeaponDamage(WeaponData.Damage);
}

void AWeaponFirearm::OnReload_Implementation()
{
}

bool AWeaponFirearm::DeployWeaponAttack_Implementation()
{
	Super::DeployWeaponAttack_Implementation();
	return Shooter->Shoot();
}
