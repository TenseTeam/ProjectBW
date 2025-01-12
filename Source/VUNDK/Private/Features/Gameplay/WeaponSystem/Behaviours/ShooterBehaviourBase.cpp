// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Behaviours/ShooterBehaviourBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter.h"

void UShooterBehaviourBase::Init(UShooter* InShooter, const FShootData InShootData)
{
	if (InShooter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Shooter in %s is null"), *GetName());
		return;
	}
	
	ShootData = InShootData;
	Shooter = InShooter;
	OnInit();
}

bool UShooterBehaviourBase::Shoot()
{
	if (bIsInCooldown)
		return false;
	
	StartShootCooldown();
	
	if (!CanShoot())
	{
		OnShootFail();
		OnBehaviourShootFail.Broadcast();
		return false;
	}

	OnShootSuccess();
	OnBehaviourShootSuccess.Broadcast();
	ModifyCurrentAmmo(-1);
	return true;
}

int32 UShooterBehaviourBase::Refill(const int32 Ammo)
{
	ModifyCurrentAmmo(Ammo);
	OnRefill();
	OnBehaviourRefill.Broadcast();
	return FMath::Abs(CurrentAmmo - Ammo);
}

void UShooterBehaviourBase::RefillAllMagazine()
{
	Refill(GetMagSize());
}

int32 UShooterBehaviourBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

float UShooterBehaviourBase::GetRange_Implementation() const
{
	if (!Check())
		return -1.0f;

	return ShootData.Range;
}

float UShooterBehaviourBase::GetFireRate_Implementation() const
{
	if (!Check())
		return -1.0f;

	return ShootData.FireRate;
}

float UShooterBehaviourBase::GetDamage_Implementation() const
{
	if (!Check())
		return -1.0f;

	return ShootData.Damage;
}

int32 UShooterBehaviourBase::GetMagSize_Implementation() const
{
	if (!Check())
		return -1;

	return ShootData.MagSize;
}

bool UShooterBehaviourBase::CanShoot() const
{
	if (!Check())
	{
		UE_LOG(LogShooter, Error, TEXT("Trying to shoot with invalid ShootData in %s."), *GetName());
		return false;
	}
	
	return bHasInfiniteAmmo || CurrentAmmo > 0;
}

void UShooterBehaviourBase::OnInit_Implementation()
{
}

void UShooterBehaviourBase::OnShootSuccess_Implementation()
{
}

void UShooterBehaviourBase::OnShootFail_Implementation()
{
}

void UShooterBehaviourBase::OnRefill_Implementation()
{
}

void UShooterBehaviourBase::SetCurrentAmmo(const int32 NewAmmoValue)
{
	CurrentAmmo = FMath::Clamp(NewAmmoValue, 0, GetMagSize());
}

void UShooterBehaviourBase::ModifyCurrentAmmo(const int32 AmmoValue)
{
	SetCurrentAmmo(CurrentAmmo + AmmoValue);
}

void UShooterBehaviourBase::StartShootCooldown()
{
	if (!IsValid(Shooter->GetWorld()))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour World is invalid in %s."), *GetName());
		return;
	}

	bIsInCooldown = true;
	FTimerHandle TimerHandle;
	const float Cooldown = 1.0f / (GetFireRate() / 60.0f);
	Shooter->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UShooterBehaviourBase::EndShootCooldown, Cooldown);
}

void UShooterBehaviourBase::EndShootCooldown()
{
	bIsInCooldown = false;
}

bool UShooterBehaviourBase::Check() const
{
	return IsValid(Shooter);
}
