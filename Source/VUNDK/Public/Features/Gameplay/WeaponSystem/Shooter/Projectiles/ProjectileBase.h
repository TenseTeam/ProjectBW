// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Patterns/ObjectPool/PooledActorBase.h"
#include "ProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FProjectileHitDelegate,
	const FHitResult&, ImpactResult
);

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FProjectileHitDelegate OnProjectileHitEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY()
	AActor* ProjectileInstigator;
	
private:
	float Damage;
	float Range;
	float RemainingLifeSpan;
	bool bIsProjectileAlive;

public:
	AProjectileBase();
	
	void Init(AActor* InInstigator, float InDamage, float InRange, float InSpeed, const FVector& InDirection);

	UFUNCTION(BlueprintNativeEvent)
	void InitVelocityAndLifeSpan(float InRange, float InSpeed, const FVector& InDirection);

	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector NewVelocity) const;

	UFUNCTION(BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintCallable)
	float GetRange() const;

	UFUNCTION(BlueprintCallable)
	void ApplyProjectileDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void DisposeProjectile();

	virtual void ClearPooledActor_Implementation() override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	void StartProjectileLifeSpan(const float InLifeSpan);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileLifeSpanEnd();
	
private:
	void ProcessProjectileLifeSpan(const float DeltaSeconds);
	
	UFUNCTION()
	void EndProjectileLifeSpan();
	
	UFUNCTION()
	void ProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
