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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Instanced)
	UActorPool* ProjectilePool;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float HitRadius = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ProjectileLifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ProjectileSpeed = 100.0f;

public:
	UShooterPhysicProjectileBehaviour();

protected:
	virtual void Init(UShooter* InShooter, const FShootData InShootData, const TArray<UShootPoint*> InShootPoints) override;

	virtual void OnShootSuccess_Implementation(const FVector& ShootPointLocation, const FVector& ShootPointDirection) override;
};
