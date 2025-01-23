// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterPhysicProjectileBehaviour.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"
#include "Patterns/ObjectPool/Utility/PoolsUtility.h"

UShooterPhysicProjectileBehaviour::UShooterPhysicProjectileBehaviour(): ProjectilePool(nullptr)
{
}

void UShooterPhysicProjectileBehaviour::Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel)
{
	ProjectilePool = UPoolsUtility::GetPool(ProjectilesPoolName);
	Super::Init(InShooter, InShootData, InShootBarrel);
}

void UShooterPhysicProjectileBehaviour::OnDeployShoot_Implementation(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const
{
	AActor* ActorPrj = ProjectilePool->AcquireActor();

	if (!IsValid(ActorPrj))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterPhysicProjectileBehaviour ShootSuccess(), Invalid projectile."));
		return;
	}

	FVector ProjectileDirection = DirectionToTarget;
	AProjectileBase* Projectile = Cast<AProjectileBase>(ActorPrj);
	Projectile->SetActorLocation(ShootPoint->GetShootPointLocation());
	Projectile->Init(Shooter->GetOwner(), GetDamage(), GetMaxRange(), ProjectileSpeed, DirectionToTarget);
}

bool UShooterPhysicProjectileBehaviour::Check() const
{
	return Super::Check() && IsValid(ProjectilePool);
}
