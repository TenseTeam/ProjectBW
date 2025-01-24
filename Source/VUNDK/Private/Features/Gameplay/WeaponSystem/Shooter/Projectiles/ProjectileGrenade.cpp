// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileGrenade.h"
#include "Kismet/GameplayStatics.h"

AProjectileGrenade::AProjectileGrenade()
{
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
}

void AProjectileGrenade::InitVelocityAndLifeSpan_Implementation(float InRange, float InSpeed, const FVector& InDirection)
{
	SetVelocity(InDirection * InSpeed);
	StartProjectileLifeSpan(TimeToExplosion);
}

float AProjectileGrenade::GetExplosionRadius() const
{
	return GetRange() / 2.f;
}

void AProjectileGrenade::Explode()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(ProjectileInstigator);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), GetDamage(), GetActorLocation(), GetExplosionRadius(), UDamageType::StaticClass(), IgnoredActors, this, ProjectileInstigator->GetInstigatorController(), bDoFullDamage, ExplosionPreventionChannel);
	OnExplosion();
}

void AProjectileGrenade::OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
}

void AProjectileGrenade::OnProjectileLifeSpanEnd_Implementation()
{
	Explode();
}

void AProjectileGrenade::OnExplosion_Implementation()
{
	DisposeProjectile();
}
