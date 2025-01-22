// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBehaviourBase.h"
#include "Patterns/ObjectPool/ActorPool.h"
#include "ShooterPhysicProjectileBehaviour.generated.h"

UCLASS()
class VUNDK_API UShooterPhysicProjectileBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ProjectilesPoolName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ProjectileSpeed = 1000.0f;

private:
	UPROPERTY()
	UActorPool* ProjectilePool;

public:
	UShooterPhysicProjectileBehaviour();

	virtual void Init(UShooter* InShooter, const FShootData InShootData, const UShootBarrel* InShootBarrel) override;

protected:
	virtual void OnDeployShoot_Implementation(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const override;

	virtual bool Check() const override;
};
