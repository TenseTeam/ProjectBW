// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBullet.h"

AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectileBullet::OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	Super::OnProjectileHit_Implementation(ImpactResult, ImpactVelocity);
	DisposeProjectile();
}
