// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "Engine/DamageEvents.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

void UShooterTraceBehaviour::OnShootSuccess_Implementation(UShootPoint* ShootPoint, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid"));
		return;
	}
	
	if (bUseCameraTargetLocation)
		TraceFromCamera(World, ShootPoint);
	else
		TraceFromShootPoint(World, ShootPoint);
}

void UShooterTraceBehaviour::OnLineTraceDamage_Implementation(const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const
{
}

void UShooterTraceBehaviour::TraceFromCamera(const UWorld* World, const UShootPoint* ShootPoint) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::TraceFromCamera: ShootPoint is not valid"));
		return;
	}
	
	FVector CameraStartPoint;
	FVector CameraEndPoint;
	FVector CameraHitPoint;

	if (!TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint))
	{
		TraceFromShootPoint(World, ShootPoint); // Fallback option
		return;
	}

	const FVector ShootPointLocation = ShootPoint->GetShootPointLocation();
	FVector TraceStartPoint = CameraStartPoint;
	FVector TraceEndPoint = CameraEndPoint;

	bool bIsInLine;
	if (bIsInLine = IsInLineOfSight(ShootPointLocation, CameraHitPoint); !bIsInLine)
	{
		TraceStartPoint = ShootPointLocation;
		TraceEndPoint = CameraHitPoint;
	}

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, ShootPointLocation, CameraHitPoint, bIsInLine ? FColor::Green : FColor::Red, false, 5.0f, 0, 1.0f);
#endif

	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::TraceFromShootPoint(const UWorld* World, const UShootPoint* ShootPoint) const
{
	if (!IsValid(ShootPoint))
	{
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::TraceFromCamera: ShootPoint is not valid"));
		return;
	}
	
	const FVector TraceStartPoint = ShootPoint->GetShootPointLocation();
	const FVector TraceEndPoint = TraceStartPoint + ShootPoint->GetShootPointDirection() * GetRange();

	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = false;

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, 5.0f, 0, 1.0f);
#endif

	TArray<FHitResult> HitResults;
	World->LineTraceMultiByChannel(HitResults, TraceStartPoint, TraceEndPoint, GetDamageChannel(), CollisionQueryParams);

	if (HitResults.Num() > 0)
	{
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
			HitResult.GetActor()->TakeDamage(GetDamage(), DamageEvent, Shooter->GetOwner()->GetInstigatorController(), Shooter->GetOwner());
			DamageHitResults.Add(HitResult);

#if WITH_EDITORONLY_DATA
			if (bDrawDebugTraceLines)
				DrawDebugBox(World, HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, 5.0f, 0, 1.0f);
#endif
		}

		OnLineTraceDamage(HitResults, DamageHitResults);
	}
}
