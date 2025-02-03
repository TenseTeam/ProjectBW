// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBehaviourBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Projectiles/ProjectileBase.h"
#include "Patterns/ObjectPool/ActorPool.h"
#include "ShooterPhysicProjectileBehaviour.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnProjectileSpawned,
	AProjectileBase*, Projectile
);

UCLASS()
class VUNDK_API UShooterPhysicProjectileBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnProjectileSpawned OnProjectileSpawned;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ProjectileSpeed = 1000.0f;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ProjectilesPoolName;

private:
	UPROPERTY()
	UActorPool* ProjectilePool;

public:
	UShooterPhysicProjectileBehaviour();

	virtual void Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel) override;

	UFUNCTION(BlueprintCallable)
	void SetProjectilesPoolName(const FName InProjectilesPoolName);

protected:
	virtual void OnDeployShoot_Implementation(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget) const override;

	virtual bool Check() const override;

private:
	void SpawnProjectile(const UShootPoint* ShootPoint, const FVector& DirectionToTarget) const;
};
