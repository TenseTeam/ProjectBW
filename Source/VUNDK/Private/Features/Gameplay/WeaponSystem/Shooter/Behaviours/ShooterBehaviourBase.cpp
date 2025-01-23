// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterBehaviourBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterBehaviourBase::Init(UShooter* InShooter, const FShootData InShootData, const UShootBarrel* InShootBarrel)
{
	if (!IsValid(InShooter))
	{
		UE_LOG(LogShooter, Error, TEXT("Shooter in %s is null."), *GetName());
		return;
	}

	if (!IsValid(InShootBarrel))
	{
		UE_LOG(LogShooter, Error, TEXT("ShootBarrel in %s is null."), *GetName());
		return;
	}

	ShootData = InShootData;
	Shooter = InShooter;
	ShootPoints = InShootBarrel->GetShootPointsChildren();
	EnableBehaviour();
	OnInit();
}

void UShooterBehaviourBase::SetOwner(APawn* InOwner)
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogShooter, Error, TEXT("Owner in %s is null."), *GetName());
		return;
	}

	Owner = InOwner;
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

bool UShooterBehaviourBase::Shoot()
{
	if (!Check())
	{
		UE_LOG(LogShooter, Error, TEXT("Shoot() in %s failed check."), *GetName());
		return false;
	}

	if (bIsInCooldown)
		return false;

	bool bSuccess;
	switch (GetShootType())
	{
	case EShootType::Simultaneous:
		{
			bSuccess = HandleSimultaneousShoot();
			break;
		}

	case EShootType::Sequential:
		{
			bSuccess = HandleSequentialShoot();
			break;
		}

	default:
		return false;
	}

	if (bSuccess)
		StartShootCooldown();
	
	return bSuccess;
}

void UShooterBehaviourBase::ResetRecoil()
{
	ShotsFired = 0;
	ImpulseRecoil = FRotator::ZeroRotator;
}

int32 UShooterBehaviourBase::Refill(const int32 Ammo)
{
	ModifyCurrentAmmo(Ammo);
	OnRefill();
	OnBehaviourRefill.Broadcast(CurrentAmmo);
	return FMath::Abs(CurrentAmmo - Ammo);
}

void UShooterBehaviourBase::RefillAllMagazine()
{
	Refill(GetMagSize());
}

void UShooterBehaviourBase::SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength)
{
	SetDamage(NewDamage);
	SetFireRate(NewFireRate);
	SetMaxRange(NewRange);
	SetMagSize(NewMagSize);
	SetRecoilStrength(NewRecoilStrength);
}

void UShooterBehaviourBase::SetDamage(const float NewDamage)
{
	ShootData.Damage = FMath::Clamp(NewDamage, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetFireRate(const float NewFireRate)
{
	ShootData.FireRate = FMath::Clamp(NewFireRate, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMaxRange(const float NewRange)
{
	ShootData.MaxRange = FMath::Clamp(NewRange, 0.f, MAX_FLT);
}

void UShooterBehaviourBase::SetMagSize(const int32 NewMagSize)
{
	ShootData.MagSize = FMath::Clamp(NewMagSize, 0, MAX_int32);
}

void UShooterBehaviourBase::SetRecoilStrength(float NewRecoilStrength)
{
	NewRecoilStrength = FMath::Clamp(NewRecoilStrength, 0.f, 1.f);
	ShootData.RecoilStrength = NewRecoilStrength;
}

void UShooterBehaviourBase::SetMaxSpread(const float NewSpread)
{
	ShootData.MaxSpread = FMath::Clamp(NewSpread, 0.f, 1.f);
}

void UShooterBehaviourBase::SetCurrentAmmo(const int32 NewAmmo)
{
	CurrentAmmo = FMath::Clamp(NewAmmo, 0, GetMagSize());
}

float UShooterBehaviourBase::GetMaxRange_Implementation() const
{
	return ShootData.MaxRange * RangeMultiplier;
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

float UShooterBehaviourBase::GetRecoilStrength_Implementation() const
{
	return ShootData.RecoilStrength * RecoilStrengthMultiplier;
}

float UShooterBehaviourBase::GetMaxSpread_Implementation() const
{
	return ShootData.MaxSpread * MaxSpreadMultiplier;
}

EShootType UShooterBehaviourBase::GetShootType() const
{
	return ShootData.ShootType;
}

int32 UShooterBehaviourBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 UShooterBehaviourBase::GetAmmoToConsume() const
{
	return AmmoToConsumePerShot;
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

void UShooterBehaviourBase::ShootSuccess(UShootPoint* ShootPoint)
{
	const FVector ShooterTargetLocation = GetShooterTargetLocation();
	FVector ShootPointDirToTarget = ShooterTargetLocation - ShootPoint->GetShootPointLocation();
	ShootPointDirToTarget.Normalize();
	ShootPoint->SetSpread(GenerateSpread());
	OnDeployShoot(ShootPoint, bUseCameraTargetLocation, ShooterTargetLocation, ShootPointDirToTarget);
	OnShootSuccess(ShootPoint, ShooterTargetLocation, ShootPointDirToTarget);
	ApplyRecoilImpulse();
	OnBehaviourShootSuccess.Broadcast(ShootPoint);
	ShotsFired++;
}

void UShooterBehaviourBase::ShootFail()
{
	OnShootFail();
	OnBehaviourShootFail.Broadcast();
}

void UShooterBehaviourBase::TickBehaviour(const float DeltaTime)
{
	OnTickBehaviour(DeltaTime);
	ProcessCooldown(DeltaTime);
	ProcessRecoilImpulseRotation(DeltaTime);
}

void UShooterBehaviourBase::OnTickBehaviour_Implementation(const float DeltaTime)
{
}

void UShooterBehaviourBase::OnBehaviourEnabled_Implementation()
{
}

void UShooterBehaviourBase::OnBehaviourDisabled_Implementation()
{
}

void UShooterBehaviourBase::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const
{
}

void UShooterBehaviourBase::OnShootSuccess_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget) const
{
}

void UShooterBehaviourBase::OnInit_Implementation()
{
}

void UShooterBehaviourBase::OnShootFail_Implementation()
{
}

void UShooterBehaviourBase::OnRefill_Implementation()
{
}

FVector UShooterBehaviourBase::GetShooterTargetLocation_Implementation() const
{
	FRotator Rotation;
	if (FVector CameraStartPoint, CameraEndPoint, CameraHitPoint; TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint, Rotation))
		return CameraHitPoint;
	
	return FVector::ZeroVector;
}

bool UShooterBehaviourBase::TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, const FVector StartPointOffset) const
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

	const FVector CameraLocation = CameraManager->GetCameraCacheView().Location;
	OutRotation = CameraManager->GetCameraCacheView().Rotation;

	const FVector WorldOffset = OutRotation.RotateVector(StartPointOffset);
	OutStartPoint = CameraLocation + WorldOffset;
	OutEndPoint = OutStartPoint + OutRotation.Vector() * GetMaxRange();
	OutHitPoint = OutEndPoint;

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, OutStartPoint, OutEndPoint, SightTraceChannel))
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

	if (FHitResult HitResult; World->LineTraceSingleByChannel(HitResult, StartPoint, TargetPoint, SightTraceChannel))
		return HitResult.ImpactPoint.Equals(TargetPoint, Tolerance);

	return true;
}

bool UShooterBehaviourBase::Check() const
{
	return IsValid(Shooter) && bIsBehaviourActive;
}

bool UShooterBehaviourBase::HandleSimultaneousShoot()
{
	if (!HasEnoughAmmoToShoot(GetAmmoToConsume()))
	{
		ShootFail();
		return false;
	}
	
	ModifyCurrentAmmo(-GetAmmoToConsume());
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
	if (!HasEnoughAmmoToShoot(GetAmmoToConsume()))
	{
		ShootFail();
		return false;
	}

	ModifyCurrentAmmo(-GetAmmoToConsume());
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

void UShooterBehaviourBase::ModifyCurrentAmmo(const int32 AmmoValue)
{
	SetCurrentAmmo(CurrentAmmo + AmmoValue);
}

void UShooterBehaviourBase::ApplyRecoilImpulse()
{
	if (!ShootData.bHasRecoil || !IsValid(ShootData.RecoilCurve))
		return;

	if (!IsValid(Owner))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterBehaviourBase::ApplyRecoilImpulse: Owner is null in %s."), *GetName());
		return;
	}

	const float RecoilPitch = ShootData.RecoilCurve->GetVectorValue(ShotsFired).Y;
	const float RecoilYaw = ShootData.RecoilCurve->GetVectorValue(ShotsFired).Z;
	ImpulseRecoil = FRotator(RecoilPitch, RecoilYaw, 0.0f);
	RecoilRemaining = ShootData.RecoilImpulseDuration;
}

void UShooterBehaviourBase::ProcessRecoilImpulseRotation(const float DeltaTime)
{
	if (!IsValid(Owner) || RecoilRemaining <= 0.0f)
		return;
	
	const FRotator RecoilStep = ImpulseRecoil * (DeltaTime * GetRecoilStrength());
	Owner->AddControllerPitchInput(-RecoilStep.Pitch);
	Owner->AddControllerYawInput(RecoilStep.Yaw);
	RecoilRemaining -= DeltaTime;
}

void UShooterBehaviourBase::StartShootCooldown()
{
	if (!IsValid(Shooter->GetWorld()))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterBehaviour World is invalid in %s."), *GetName());
		return;
	}

	const float Cooldown = 1.0f / (GetFireRate() / 60.0f);
	CooldownRemaining = Cooldown;
	bIsInCooldown = true;
}

void UShooterBehaviourBase::ProcessCooldown(const float DeltaTime)
{
	if (!bIsInCooldown)
		return;

	CooldownRemaining -= DeltaTime;
	if (CooldownRemaining <= 0.0f)
		EndShootCooldown();
}

void UShooterBehaviourBase::EndShootCooldown()
{
	bIsInCooldown = false;
}

float UShooterBehaviourBase::GenerateSpread() const
{
	return FMath::RandRange(0.f, GetMaxSpread());
}
