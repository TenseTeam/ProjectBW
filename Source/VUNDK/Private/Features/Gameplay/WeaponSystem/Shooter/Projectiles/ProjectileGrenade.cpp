// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileGrenade.h"

AProjectileGrenade::AProjectileGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileGrenade::OnPooledActorBeginPlay_Implementation()
{
	Super::OnPooledActorBeginPlay_Implementation();
}

void AProjectileGrenade::OnPooledActorEndPlay_Implementation()
{
	Super::OnPooledActorEndPlay_Implementation();
}

void AProjectileGrenade::OnProjectileLifeSpanEnd_Implementation()
{
	Super::OnProjectileLifeSpanEnd_Implementation();
	OnExplosion();
}

void AProjectileGrenade::OnExplosion_Implementation()
{
	RadialDamage();
}
