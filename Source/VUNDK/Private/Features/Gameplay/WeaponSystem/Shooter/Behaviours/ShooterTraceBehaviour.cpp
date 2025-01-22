// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "Engine/DamageEvents.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

void UShooterTraceBehaviour::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid."));
		return;
	}

	switch (ShootTraceMode)
	{
	case EShootTraceMode::CameraSightTrace:
		CameraSightTrace(World, ShootPoint);
		return;
	case EShootTraceMode::ShootPointTrace:
		ShootPointTrace(World, ShootPoint);
	default:
		break;
	}
}

void UShooterTraceBehaviour::OnHitResults_Implementation(const FVector& HitLocation, const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const
{
}

void UShooterTraceBehaviour::CameraSightTrace(const UWorld* World, const UShootPoint* ShootPoint) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::CameraOriginTrace: ShootPoint is not valid."));
		return;
	}

	FVector CameraStartPoint;
	FVector CameraHitPoint;
	FVector CameraEndPoint;
	FRotator CameraRotation;
	if (!TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint, CameraRotation, ShootPoint->GetShootPointRelativeLocation()))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::CameraOriginTrace: TryGetCameraPoints failed, using Fallback option."));
		ShootPointTrace(World, ShootPoint); // Fallback option
		return;
	}

	const FVector ShootPointLocation = ShootPoint->GetShootPointLocation();
	const FVector WorldShootDirection = CameraRotation.RotateVector(ShootPoint->GetShootPointRelativeDirection());
	FVector TraceStartPoint = CameraStartPoint;
	FVector TraceEndPoint = TraceStartPoint + WorldShootDirection * GetRange();

	const bool bIsInLineOfSight = IsInLineOfSight(ShootPointLocation, CameraHitPoint);
	if (!bIsInLineOfSight)
	{
		TraceStartPoint = ShootPointLocation;
		TraceEndPoint = ShootPointLocation + ShootPoint->GetShootPointDirection() * GetRange();
	}

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, ShootPointLocation, CameraHitPoint, bIsInLineOfSight ? FColor::Green : FColor::Red, false, DebugTraceLineDuration, 0, 1.0f);
#endif
	
	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::ShootPointTrace(const UWorld* World, const UShootPoint* ShootPoint) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::ShootPointTrace: ShootPoint is not valid."));
		return;
	}

	const FVector TraceStartPoint = ShootPoint->GetShootPointLocation();
	const FVector TraceEndPoint = TraceStartPoint + ShootPoint->GetShootPointDirection() * GetRange();

	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const
{
	FVector HitLocation = TraceEndPoint;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = false;

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, DebugTraceLineDuration, 0, 1.0f);
#endif

	TArray<FHitResult> HitResults;
	World->LineTraceMultiByChannel(HitResults, TraceStartPoint, TraceEndPoint, SightTraceChannel, CollisionQueryParams);

	if (HitResults.Num() > 0)
	{
		HitLocation = HitResults[0].ImpactPoint;
		TArray<FHitResult> DamageHitResults;
		int32 PenetrationCount = 0;

		for (const FHitResult& HitResult : HitResults)
		{
			if (!IsValid(HitResult.GetActor()))
				continue;

			if (PenetrationCount >= MaxPenetration)
				break;

			PenetrationCount++;
			FDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = UDamageType::StaticClass();
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), GetDamage(), HitResult.ImpactNormal, HitResult, Shooter->GetOwner()->GetInstigatorController(), Shooter->GetOwner(), UDamageType::StaticClass());
			DamageHitResults.Add(HitResult);

#if WITH_EDITORONLY_DATA
			if (bDrawDebugTraceLines)
				DrawDebugBox(World, HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, DebugTraceLineDuration, 0, 1.0f);
#endif
		}

		OnHitResults(HitLocation, HitResults, DamageHitResults);
	}
}
