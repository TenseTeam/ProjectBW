// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "Engine/DamageEvents.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

void UShooterTraceBehaviour::OnShootSuccess_Implementation(const FVector& ShootPointLocation, const FVector& ShootPointDirection, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid"));
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid"));
		return;
	}

	if (bUseCameraTargetLocation)
		TraceFromCamera(World, ShootPointLocation);
	else
		TraceFromShootPoint(World, ShootPointLocation, ShootPointDirectionToTarget);
}

void UShooterTraceBehaviour::TraceFromCamera(const UWorld* World, const FVector& InShootPointLocation) const
{
	FVector CameraStartPoint;
	FVector CameraEndPoint;
	FVector CameraHitPoint;

	if (!TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint))
		return;

	FVector TraceStartPoint = CameraStartPoint;
	FVector TraceEndPoint = CameraEndPoint;

	bool bIsInLine;
	if (bIsInLine = IsInLineOfSight(InShootPointLocation, CameraHitPoint); !bIsInLine)
	{
		TraceStartPoint = InShootPointLocation;
		TraceEndPoint = CameraHitPoint;
	}

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTraceLines)
		DrawDebugLine(World, InShootPointLocation, CameraHitPoint, bIsInLine ? FColor::Green : FColor::Red, false, 5.0f, 0, 1.0f);
#endif

	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::TraceFromShootPoint(const UWorld* World, const FVector& InShootPointLocation, const FVector& ShootPointDirectionToTarget) const
{
	const FVector TraceStartPoint = InShootPointLocation;
	const FVector TraceEndPoint = InShootPointLocation + ShootPointDirectionToTarget * GetRange();

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

	if (TArray<FHitResult> HitResults; World->LineTraceMultiByProfile(HitResults, TraceStartPoint, TraceEndPoint, TraceProfile.Name, CollisionQueryParams))
	{
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

#if WITH_EDITORONLY_DATA
			if (bDrawDebugTraceLines)
				DrawDebugBox(World, HitResult.ImpactPoint, FVector(5.0f), FColor::Red, false, 5.0f, 0, 1.0f);
#endif
		}
	}
}
