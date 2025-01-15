// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/GrenadeProjectile.h"

AGrenadeProjectile::AGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGrenadeProjectile::OnPooledActorBeginPlay_Implementation()
{
	Super::OnPooledActorBeginPlay_Implementation();
}

void AGrenadeProjectile::OnPooledActorEndPlay_Implementation()
{
	Super::OnPooledActorEndPlay_Implementation();
}

void AGrenadeProjectile::OnProjectileLifeSpanEnd_Implementation()
{
	Super::OnProjectileLifeSpanEnd_Implementation();
	OnExplosion();
}

void AGrenadeProjectile::OnExplosion_Implementation()
{
	RadialDamage();
}
