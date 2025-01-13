// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Behaviours/ShooterPhysicProjectileBehaviour.h"

#include "Features/Gameplay/WeaponSystem/Shooter.h"
#include "Features/Gameplay/WeaponSystem/Projectiles/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

UShooterPhysicProjectileBehaviour::UShooterPhysicProjectileBehaviour(): ProjectilePool(nullptr)
{
	ProjectilePool = CreateDefaultSubobject<UActorPool>(TEXT("ProjectilePool"));
	ProjectilePool->SetActorClass(AProjectileBase::StaticClass());
}

void UShooterPhysicProjectileBehaviour::Init(UShooter* InShooter, const FShootData InShootData)
{
	Super::Init(InShooter, InShootData);
	ProjectilePool->Init();
}

void UShooterPhysicProjectileBehaviour::ShootSuccess()
{
	Super::ShootSuccess();

	// TODO: Implement
	AActor* Prj = ProjectilePool->AcquireActor();

	if (!IsValid(Prj))
	{
		UE_LOG(LogShooter, Error, TEXT("ShooterPhysicProjectileBehaviour ShootSuccess(), Invalid projectile."));
		return;
	}
	
	const FVector Start = FVector(FMath::RandRange(0, 10), FMath::RandRange(0, 10), FMath::RandRange(0, 10));
	Prj->SetActorLocation(Start);
}
