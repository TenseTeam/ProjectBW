// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileGrenade.h"

#include "Kismet/GameplayStatics.h"

AProjectileGrenade::AProjectileGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileGrenade::ApplyExplosionDamage()
{
	const UWorld* World = GetWorld();

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(ProjectileInstigator);
	UGameplayStatics::ApplyRadialDamage(World, GetDamage(), GetActorLocation(), ExplosionRadius, UDamageType::StaticClass(), IgnoredActors, this, ProjectileInstigator->GetInstigatorController(), true, ExplosionPreventionChannel);
}

void AProjectileGrenade::OnProjectileLifeSpanEnd_Implementation()
{
	Super::OnProjectileLifeSpanEnd_Implementation();
	OnExplosion();
}

void AProjectileGrenade::OnExplosion_Implementation()
{
	ApplyExplosionDamage();
}
