// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

UShooter::UShooter(): ShooterBehaviour(nullptr),
                      ShootData()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShooter::Init(const TArray<UShootPoint*> InShootPoints)
{
	ShootPoints = InShootPoints;
	
	if (Check())
		ShooterBehaviour->Init(this, ShootData, ShootPoints);
}

bool UShooter::Shoot(const EShootType ShootType) const
{
	if (!Check())
		return false;
	
	return ShooterBehaviour->Shoot(ShootType);
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

void UShooter::SetShooterDamage(const float NewDamage) const
{
	if (!Check())
		return;
	
	ShooterBehaviour->SetDamage(NewDamage);
}

int32 UShooter::GetCurrentAmmo() const
{
	if (!Check())
		return -1;
	
	return ShooterBehaviour->GetCurrentAmmo();
}

bool UShooter::Check() const
{
	return IsValid(ShooterBehaviour);
}
