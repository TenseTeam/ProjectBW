// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

UShooter::UShooter(): ShooterBehaviour(nullptr),
                      ShootData()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShooter::Init(UShootBarrel* InShootBarrel)
{
	if (!Check())
		return;
	
	ShooterBehaviour->Init(this, ShootData, InShootBarrel);
}

void UShooter::SetOwner(APawn* InOwner) const
{
	ShooterBehaviour->SetOwner(InOwner);
}

bool UShooter::Shoot() const
{
	if (!Check())
		return false;
	
	return ShooterBehaviour->Shoot();
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

void UShooter::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Check())
		return;
	ShooterBehaviour->TickBehaviour(DeltaTime);
}

bool UShooter::Check() const
{
	return IsValid(ShooterBehaviour);
}
