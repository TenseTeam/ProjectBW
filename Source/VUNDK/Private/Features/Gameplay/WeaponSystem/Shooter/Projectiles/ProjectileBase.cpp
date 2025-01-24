// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(MeshComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 200.f;
}

void AProjectileBase::Init(AActor* InInstigator, const float InDamage, const float InRange, const float InSpeed, const FVector& InDirection)
{
	ProjectileInstigator = InInstigator;
	Damage = InDamage;
	Range = InRange;
	ProjectileMovementComponent->bShouldBounce = true;
	InitVelocityAndLifeSpan(Range, InSpeed, InDirection);
}

void AProjectileBase::InitVelocityAndLifeSpan_Implementation(float InRange, float InSpeed, const FVector& InDirection)
{
	SetVelocity(InDirection * InSpeed);
	StartProjectileLifeSpan(InRange / InSpeed);
}

void AProjectileBase::SetVelocity(const FVector NewVelocity) const
{
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->Velocity = NewVelocity;
}

float AProjectileBase::GetDamage() const
{
	return Damage;
}

float AProjectileBase::GetRange() const
{
	return Range;
}

void AProjectileBase::ApplyProjectileDamage(AActor* TargetActor)
{
	const UWorld* World = GetWorld();

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(ProjectileInstigator);

	UGameplayStatics::ApplyDamage(TargetActor, GetDamage(), ProjectileInstigator->GetInstigatorController(), ProjectileInstigator, UDamageType::StaticClass());
}

void AProjectileBase::DisposeProjectile()
{
	if (!bIsProjectileAlive)
		return;
	
	IPooledActor::Execute_ReleasePooledActor(this);
}

void AProjectileBase::ClearPooledActor_Implementation()
{
	ProjectileMovementComponent->StopMovementImmediately();
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileBase::OnProjectileHit);
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ProjectileMovementComponent->OnProjectileBounce.RemoveDynamic(this, &AProjectileBase::OnProjectileHit);
}

void AProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessProjectileLifeSpan(DeltaSeconds);
}

void AProjectileBase::OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	ApplyProjectileDamage(ImpactResult.GetActor());
}

void AProjectileBase::OnProjectileLifeSpanEnd_Implementation()
{
}

void AProjectileBase::StartProjectileLifeSpan(const float InLifeSpan)
{
	RemainingLifeSpan = InLifeSpan;
	bIsProjectileAlive = true;
}

void AProjectileBase::ProcessProjectileLifeSpan(const float DeltaSeconds)
{
	if (!bIsProjectileAlive)
		return;

	RemainingLifeSpan -= DeltaSeconds;

	if (RemainingLifeSpan <= 0.f)
		EndProjectileLifeSpan();
}

void AProjectileBase::EndProjectileLifeSpan()
{
	OnProjectileLifeSpanEnd();
	DisposeProjectile();
	bIsProjectileAlive = false;
}

void AProjectileBase::ProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	OnProjectileHit(ImpactResult, ImpactVelocity);
	OnProjectileHitEvent.Broadcast(ImpactResult);
}
