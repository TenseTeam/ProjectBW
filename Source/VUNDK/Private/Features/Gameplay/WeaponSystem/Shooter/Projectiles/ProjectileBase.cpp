// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true);
}

void AProjectileBase::Init(AActor* InInstigator, const float InDamage, const float InLifeSpan, const float InHitRadius, TEnumAsByte<ECollisionChannel> InBlockingChannel, const FVector& InVelocity)
{
	ProjectileInstigator = InInstigator;
	Damage = InDamage;
	LifeSpan = InLifeSpan;
	HitRadius = InHitRadius;
	BlockingChannel = InBlockingChannel;
	SetVelocity(InVelocity);
	SetProjectileLifeSpan(LifeSpan);
}

void AProjectileBase::SetVelocity(const FVector NewVelocity) const
{
	MeshComponent->SetPhysicsLinearVelocity(NewVelocity);
}

float AProjectileBase::GetDamage() const
{
	return Damage;
}

float AProjectileBase::GetHitRadius() const
{
	return HitRadius;
}

bool AProjectileBase::RadialDamage()
{
	const UWorld* World = GetWorld();
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(ProjectileInstigator);
	return UGameplayStatics::ApplyRadialDamage(World, GetDamage(), GetActorLocation(), GetHitRadius(), UDamageType::StaticClass(), IgnoredActors, ProjectileInstigator, ProjectileInstigator->GetInstigatorController(), true, BlockingChannel);
}

void AProjectileBase::DisposeProjectile()
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ProjectileBase DisposeProjectile(), World is invalid."));
		return;
	}
	
	World->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
	IPooledActor::Execute_ReleasePooledActor(this);
}

void AProjectileBase::ClearPooledActor_Implementation()
{
	SetVelocity(FVector::ZeroVector);
}

void AProjectileBase::SetProjectileLifeSpan(float InLifeSpan)
{
	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("ProjectileBase SetProjectileLifeSpan(), World is invalid."));
		return;
	}
	
	World->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::OnProjectileLifeSpanEnd, LifeSpan, false);
}

void AProjectileBase::OnProjectileLifeSpanEnd_Implementation()
{
	DisposeProjectile();
}
