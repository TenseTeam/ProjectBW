// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterBehaviourBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterBehaviourBase::Init(UShooter* InShooter, const FShootData InShootData, const TArray<UShootPoint*> InShootPoints)
{
	if (!IsValid(InShooter))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter in %s is null."), *GetName());
		return;
	}

	if (InShootPoints.Num() == 0)
	{
		UE_LOG(LogShooter, Error, TEXT("ShootPoints in %s is empty."), *GetName());
		return;
	}

	ShootData = InShootData;
	Shooter = InShooter;
	ShootPoints = InShootPoints;
	EnableBehaviour();
	OnInit();
}

void UShooterBehaviourBase::EnableBehaviour()
{
	bIsBehaviourActive = true;
	OnBehaviourEnabled();
}

void UShooterBehaviourBase::DisableBehaviour()
{
	bIsBehaviourActive = false;
	OnBehaviourDisabled();
}

bool UShooterBehaviourBase::Shoot(const EShootType ShootType)
{
	if (!Check())
		return false;

	if (bIsInCooldown)
		return false;

	StartShootCooldown();

	switch (ShootType)
	{
	case EShootType::Simultaneous:
		{
			HandleSimultaneousShoot();
			break;
		}

	case EShootType::Sequential:
		{
			HandleSequentialShoot();
			break;
		}

	default:
		return false;
	}

	return true;
}

int32 UShooterBehaviourBase::Refill(const int32 Ammo)
{
	if (!Check())
		return 0;

	ModifyCurrentAmmo(Ammo);
	OnRefill();
	OnBehaviourRefill.Broadcast(CurrentAmmo);
	return FMath::Abs(CurrentAmmo - Ammo);
}

void UShooterBehaviourBase::RefillAllMagazine()
{
	Refill(GetMagSize());
}

void UShooterBehaviourBase::SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize)
{
	SetDamage(NewDamage);
	SetFireRate(NewFireRate);
	SetRange(NewRange);
	SetMagSize(NewMagSize);
}

void UShooterBehaviourBase::SetDamage(const float NewDamage)
{
	if (NewDamage < 0.0f)
	{
		UE_LOG(LogShooter, Warning, TEXT("Trying to set negative damage in %s. New Damage set to zero."), *GetName());
		ShootData.Damage = 0.0f;
		return;
	}

	ShootData.Damage = NewDamage;
}

void UShooterBehaviourBase::SetFireRate(const float NewFireRate)
{
	if (NewFireRate < 0.0f)
	{
		UE_LOG(LogShooter, Warning, TEXT("Trying to set negative fire rate in %s. New Fire Rate set to zero."), *GetName());
		ShootData.FireRate = 0.0f;
		return;
	}

	ShootData.FireRate = NewFireRate;
}

void UShooterBehaviourBase::SetRange(const float NewRange)
{
	if (NewRange < 0.0f)
	{
		UE_LOG(LogShooter, Warning, TEXT("Trying to set negative range in %s. New Range set to zero."), *GetName());
		ShootData.Range = 0.0f;
		return;
	}

	ShootData.Range = NewRange;
}

void UShooterBehaviourBase::SetMagSize(const int32 NewMagSize)
{
	if (NewMagSize < 0)
	{
		UE_LOG(LogShooter, Warning, TEXT("Trying to set negative mag size in %s. New Mag Size set to zero."), *GetName());
		ShootData.MagSize = 0;
		return;
	}

	ShootData.MagSize = NewMagSize;
}

float UShooterBehaviourBase::GetRange_Implementation() const
{
	return ShootData.Range * 100.f;
}

float UShooterBehaviourBase::GetFireRate_Implementation() const
{
	return ShootData.FireRate;
}

float UShooterBehaviourBase::GetDamage_Implementation() const
{
	return ShootData.Damage;
}

int32 UShooterBehaviourBase::GetMagSize_Implementation() const
{
	return ShootData.MagSize;
}

int32 UShooterBehaviourBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

TEnumAsByte<ECollisionChannel> UShooterBehaviourBase::GetDamageChannel() const
{
	return ShootData.DamageChannel;
}

UWorld* UShooterBehaviourBase::GetWorld() const
{
	if (!IsValid(Shooter))
		return Super::GetWorld();
	
	return Shooter->GetWorld();
}

#if WITH_EDITOR
bool UShooterBehaviourBase::ImplementsGetWorld() const
{
	// Return true so in Editor we can see WorldContexts Functions
	return true;
}
#endif

void UShooterBehaviourBase::ShootSuccess(UShootPoint* ShootPoint) const
{
	const FVector ShooterTargetLocation = GetShooterTargetLocation();
	FVector ShootPointDirToTarget = ShooterTargetLocation - ShootPoint->GetShootPointLocation();
	ShootPointDirToTarget.Normalize();
	OnShootSuccess(ShootPoint, ShooterTargetLocation, ShootPointDirToTarget);
	OnBehaviourShootSuccess.Broadcast(ShootPoint);
}

void UShooterBehaviourBase::ShootFail()
{
	OnShootFail();
	OnBehaviourShootFail.Broadcast();
}

void UShooterBehaviourBase::OnBehaviourEnabled_Implementation()
{
}

void UShooterBehaviourBase::OnBehaviourDisabled_Implementation()
{
}

void UShooterBehaviourBase::OnInit_Implementation()
{
}

void UShooterBehaviourBase::OnShootSuccess_Implementation(UShootPoint* ShootPoint, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const
{
}

void UShooterBehaviourBase::OnShootFail_Implementation()
{
}

void UShooterBehaviourBase::OnRefill_Implementation()
{
}

FVector UShooterBehaviourBase::CalculateShooterTargetLocation_Implementation() const
{
	return FVector::ZeroVector;
}

FVector UShooterBehaviourBase::GetShooterTargetLocation() const
{
	if (!bUseCameraTargetLocation)
		return CalculateShooterTargetLocation();

	FVector CameraStartPoint;
	FVector CameraEndPoint;
	FVector CameraHitPoint;
	if (TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint))
		return CameraHitPoint;

	return FVector::ZeroVector;
}

bool UShooterBehaviourBase::TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint) const
{
	const UWorld* World = Shooter->GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour LineTraceFromCamera(), World is invalid in %s."), *GetName());
		return false;
	}

	const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!IsValid(CameraManager))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour LineTraceFromCamera(), CameraManager is null in %s."), *GetName());
		return false;
	}

	OutStartPoint = CameraManager->GetCameraCacheView().Location;
	OutEndPoint = OutStartPoint + CameraManager->GetCameraCacheView().Rotation.Vector() * GetRange();
	OutHitPoint = OutEndPoint;

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, OutStartPoint, OutEndPoint, ECollisionChannel::ECC_Camera))
		OutHitPoint = HitResult.ImpactPoint;

	return true;
}

bool UShooterBehaviourBase::IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour IsInLineOfSight(), World is invalid in %s."), *GetName());
		return false;
	}

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, StartPoint, TargetPoint, ECC_Visibility))
		return HitResult.ImpactPoint.Equals(TargetPoint, Tolerance);

	return true;
}

bool UShooterBehaviourBase::HandleSimultaneousShoot()
{
	if (!HasEnoughAmmoToShoot(ShootPoints.Num()))
	{
		ShootFail();
		return false;
	}

	ModifyCurrentAmmo(-ShootPoints.Num());
	for (UShootPoint* ShootPoint : ShootPoints)
	{
		if (!IsValid(ShootPoint))
		{
			UE_LOG(LogShooter, Error, TEXT("HandleSimultaneousShoot(), Invalid ShootPoint in %s."), *GetName());
			continue;
		}

		ShootSuccess(ShootPoint);
	}

	return true;
}

bool UShooterBehaviourBase::HandleSequentialShoot()
{
	if (!HasEnoughAmmoToShoot(1))
	{
		ShootFail();
		return false;
	}

	ModifyCurrentAmmo(-1);
	NextShootPointIndex();

	if (!IsValid(ShootPoints[CurrentShootPointIndex]))
	{
		UE_LOG(LogShooter, Error, TEXT("HandleSequentialShoot(), Invalid ShootPoint in %s."), *GetName());
		return false;
	}

	ShootSuccess(ShootPoints[CurrentShootPointIndex]);

	return true;
}

bool UShooterBehaviourBase::HasEnoughAmmoToShoot(const int32 DesiredAmmoToConsume) const
{
	return ShootPoints.Num() > 0 && (bHasInfiniteAmmo || CurrentAmmo - DesiredAmmoToConsume >= 0);
}

int32 UShooterBehaviourBase::NextShootPointIndex()
{
	if (ShootPoints.Num() == 0)
	{
		UE_LOG(LogShooter, Error, TEXT("ShootPoints in %s is empty."), *GetName());
		return -1;
	}

	const int32 ShootPointIndex = (CurrentShootPointIndex + 1) % ShootPoints.Num();
	CurrentShootPointIndex = ShootPointIndex;
	return CurrentShootPointIndex;
}

void UShooterBehaviourBase::SetCurrentAmmo(const int32 NewAmmoValue)
{
	if (bHasInfiniteAmmo)
		return;

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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UShooterBehaviourBase::EndShootCooldown, Cooldown);
}

void UShooterBehaviourBase::EndShootCooldown()
{
	bIsInCooldown = false;
}

bool UShooterBehaviourBase::Check() const
{
	return IsValid(Shooter) && bIsBehaviourActive;
}
