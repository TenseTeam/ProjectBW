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
	
	if (!Check())
		return;
	
	ShooterBehaviour->Init(this, ShootData, ShootPoints);
}

void UShooter::SetOwner(APawn* InOwner) const
{
	ShooterBehaviour->SetOwner(InOwner);
}

bool UShooter::Shoot(const EShootType ShootType) const
{
	if (!Check())
		return false;
	
	return ShooterBehaviour->Shoot(ShootType);
}

void UShooter::ResetRecoil() const
{
	ShooterBehaviour->ResetRecoil();
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
	
	ShooterBehaviour->RefillAllMagazine();
}

int32 UShooter::GetCurrentAmmo() const
{
	if (!Check())
		return -1;
	
	return ShooterBehaviour->GetCurrentAmmo();
}

void UShooter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
		return;
	
	ShooterBehaviour->DisableBehaviour();
}

bool UShooter::Check() const
{
	return IsValid(ShooterBehaviour);
}
