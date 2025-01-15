// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Behaviours/ShooterPhysicProjectileBehaviour.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"

UShooterPhysicProjectileBehaviour::UShooterPhysicProjectileBehaviour(): ProjectilePool(nullptr)
{
	ProjectilePool = CreateDefaultSubobject<UActorPool>(TEXT("ProjectilePool"));
	ProjectilePool->SetActorClass(AProjectileBase::StaticClass());
}

void UShooterPhysicProjectileBehaviour::Init(UShooter* InShooter, const FShootData InShootData, const TArray<UShootPoint*> InShootPoints)
{
	Super::Init(InShooter, InShootData, InShootPoints);
	ProjectilePool->Init();
}

void UShooterPhysicProjectileBehaviour::OnShootSuccess_Implementation(UShootPoint* ShootPoint, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const
{
	AActor* ActorPrj = ProjectilePool->AcquireActor();

	if (!IsValid(ActorPrj))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterPhysicProjectileBehaviour ShootSuccess(), Invalid projectile."));
		return;
	}
	
	AProjectileBase* Projectile = Cast<AProjectileBase>(ActorPrj);
	Projectile->SetActorLocation(ShootPoint->GetShootPointLocation());
	Projectile->Init(Shooter->GetOwner(), GetDamage(), ProjectileLifeTime, HitRadius, GetDamageChannel(), ShootPointDirectionToTarget * ProjectileSpeed);
}
