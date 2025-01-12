// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter.h"

UShooter::UShooter(): ShooterBehaviour(nullptr),
                      ShootData()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UShooter::Shoot() const
{
	if (!Check())
		return false;
	
	return ShooterBehaviour->Shoot();
}

int32 UShooter::Refill(const int32 Ammo) const
{
	if (!Check())
		return -1;
	
	return ShooterBehaviour->Refill(Ammo);
}

void UShooter::RefillAllMagazine() const
{
	if (!Check())
		return;
	
	return ShooterBehaviour->RefillAllMagazine();
}

int32 UShooter::GetCurrentAmmo() const
{
	if (!Check())
		return -1;
	
	return ShooterBehaviour->GetCurrentAmmo();
}

void UShooter::BeginPlay()
{
	Super::BeginPlay();
	ShooterBehaviour->Init(this, ShootData);
}

bool UShooter::Check() const
{
	return IsValid(ShooterBehaviour);
}
