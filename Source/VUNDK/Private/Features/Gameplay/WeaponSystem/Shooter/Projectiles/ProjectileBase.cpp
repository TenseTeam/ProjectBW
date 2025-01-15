// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"

#include "Engine/DamageEvents.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->SetSimulatePhysics(true);
}

void AProjectileBase::Init(AActor* InInstigator, const float InDamage, const float InLifeSpan, const float InHitRadius, TEnumAsByte<ECollisionChannel> InDamageChannel, const FVector& InVelocity)
{
	ProjectileInstigator = InInstigator;
	Damage = InDamage;
	LifeSpan = InLifeSpan;
	HitRadius = InHitRadius;
	DamageChannel = InDamageChannel;
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

void AProjectileBase::RadialDamage() const
{
	const UWorld* World = GetWorld();
	
	if (!IsValid(World))
	{
		UE_LOG(LogShooter, Error, TEXT("SphereCastDamage(), World is invalid in %s."), *GetName());
		return;
	}

	DrawDebugSphere(World, GetActorLocation(), GetHitRadius(), 32, FColor::Red, false, 5.f, 0, 5.f);
	UGameplayStatics::ApplyRadialDamage(World, GetDamage(), GetActorLocation(), GetHitRadius(), UDamageType::StaticClass(), TArray<AActor*>(), ProjectileInstigator, ProjectileInstigator->GetInstigatorController(), true, DamageChannel);
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
		UE_LOG(LogShooter, Error, TEXT("SetProjectileLifeSpan(), World is invalid in %s."), *GetName());
		return;
	}

	FTimerHandle LifeSpanTimerHandle;
	World->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::OnProjectileLifeSpanEnd, LifeSpan, false);
}

void AProjectileBase::OnProjectileLifeSpanEnd_Implementation()
{
	IPooledActor::Execute_ReleasePooledActor(this);
}
