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

	DrawDebugLine(World, InShootPointLocation, CameraHitPoint, bIsInLine ? FColor::Green : FColor::Red, false, 5.0f, 0, 1.0f);
	DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, 5.0f, 0, 1.0f);

	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::TraceFromShootPoint(const UWorld* World, const FVector& InShootPointLocation, const FVector& ShootPointDirectionToTarget) const
{
	const FVector TraceStartPoint = InShootPointLocation;
	const FVector TraceEndPoint = InShootPointLocation + ShootPointDirectionToTarget * GetRange();

	DrawDebugLine(World, TraceStartPoint, TraceEndPoint, FColor::Purple, false, 5.0f, 0, 1.0f);
	LineTraceDamage(World, TraceStartPoint, TraceEndPoint);
}

void UShooterTraceBehaviour::LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = false;
	
	if (TArray<FHitResult> HitResults; World->LineTraceMultiByChannel(HitResults, TraceStartPoint, TraceEndPoint, GetDamageChannel(), CollisionQueryParams))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: HitResult: %d"), HitResults.Num()));

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
		}
	}
}
