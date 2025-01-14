// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterTraceBehaviour.h"
#include "KismetTraceUtils.h"
#include "Engine/DamageEvents.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

void UShooterTraceBehaviour::OnShootSuccess_Implementation(const FVector& ShootPointLocation, const FVector& ShootPointDirection)
{
	Super::OnShootSuccess_Implementation(ShootPointLocation, ShootPointDirection);
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid"));
		UE_LOG(LogShooter, Error, TEXT("UShooterTraceBehaviour::OnShootSuccess_Implementation: World is not valid"));
		return;
	}

	FVector CameraStartPoint;
	FVector CameraEndPoint;
	FVector CameraHitPoint;
	if (!TryGetCameraPoints(CameraStartPoint, CameraEndPoint, CameraHitPoint))
		return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Shooter->GetOwner());
	CollisionQueryParams.bTraceComplex = true;

	FVector TraceStartPoint = CameraStartPoint;
	FVector TraceEndPoint = CameraEndPoint;
	
	if (!IsInLineOfSight(ShootPointLocation, CameraHitPoint))
	{
		TraceStartPoint = ShootPointLocation;
		TraceEndPoint = ShootPointLocation + ShootPointDirection * GetRange();
	}

	DrawDebugLine(World, CameraStartPoint, CameraEndPoint, FColor::Red, false, 5.0f, 0, 1.0f);
	if (TArray<FHitResult> HitResults; World->LineTraceMultiByChannel(HitResults, TraceStartPoint, TraceEndPoint, GetDamageChannel(), CollisionQueryParams))
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
		}
	}
}
