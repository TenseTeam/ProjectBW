// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"

AWeaponFirearm::AWeaponFirearm()
{
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("Shooter"));
	ShootBarrel = CreateDefaultSubobject<UShootBarrel>(TEXT("ShootBarrel"));
	ShootBarrel->SetupAttachment(WeaponMesh);
}

void AWeaponFirearm::Init(APawn* InOwner, UObject* InPayload)
{
	Super::Init(InOwner, InPayload);
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

void AWeaponFirearm::SetWeaponRecoilStrength(const float NewRecoilStrength) const
{
	Shooter->ShooterBehaviour->SetRecoilStrength(NewRecoilStrength);
}

void AWeaponFirearm::SetCurrentAmmo(const int32 NewAmmo) const
{
	Shooter->ShooterBehaviour->SetCurrentAmmo(NewAmmo);
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

void AWeaponFirearm::SetAimDownSight(const bool bIsADSEnabled)
{
	if (bIsADSEnabled)
		EnableAimDownSight();
	else
		DisableAimDownSight();

	bIsAimingDownSight = bIsADSEnabled;
}

bool AWeaponFirearm::IsAimingDownSight() const
{
	return bIsAimingDownSight;
}

void AWeaponFirearm::BeginPlay()
{
	Super::BeginPlay();
	Shooter->Init(ShootBarrel);
	SetWeaponDamage(WeaponData.Damage);
	SetAimDownSightModifiers();
	AttackBarrelToSocket();
	Shooter->ShooterBehaviour->OnCurrentAmmoChanged.AddDynamic(this, &AWeaponFirearm::OnCurrentAmmoChanged);
}

void AWeaponFirearm::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Shooter->ShooterBehaviour->OnCurrentAmmoChanged.RemoveDynamic(this, &AWeaponFirearm::OnCurrentAmmoChanged);
}

void AWeaponFirearm::EnableAimDownSight()
{
	SetWeaponMaxSpread(AdsMaxSpread);
	SetWeaponRecoilStrength(AdsRecoilStrength);
	OnEnableAimDownSight();
}

void AWeaponFirearm::DisableAimDownSight()
{
	SetWeaponMaxSpread(DefaultMaxSpread);
	SetWeaponRecoilStrength(DefaultRecoilStrength);
	OnEnableAimDownSight();
}

void AWeaponFirearm::OnEnableAimDownSight_Implementation()
{
}

void AWeaponFirearm::OnReload_Implementation()
{
}

void AWeaponFirearm::OnCurrentAmmoChanged_Implementation(int32 CurrentAmmo, int32 MagSize)
{
}

void AWeaponFirearm::OnDisableAimUpSight_Implementation()
{
}

bool AWeaponFirearm::DeployWeaponAttack_Implementation()
{
	Super::DeployWeaponAttack_Implementation();
	return Shooter->Shoot();
}

void AWeaponFirearm::SetAimDownSightModifiers()
{
	DefaultMaxSpread = Shooter->ShooterBehaviour->GetMaxSpread();
	AdsMaxSpread = DefaultMaxSpread - ((DefaultMaxSpread / 100.f) * FirearmData.ADSPrecisionIncrease);

	DefaultRecoilStrength = Shooter->ShooterBehaviour->GetRecoilStrength();
	AdsRecoilStrength = DefaultRecoilStrength - ((DefaultRecoilStrength / 100.f) * FirearmData.ADSRecoilControlIncrease);
}

void AWeaponFirearm::AttackBarrelToSocket() const
{
	if (WeaponMesh->DoesSocketExist(ShootBarrelSocketName))
		ShootBarrel->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ShootBarrelSocketName);
}
