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

public:
	UShooterPhysicProjectileBehaviour();

protected:
	virtual void Init(UShooter* InShooter, const FShootData InShootData) override;

	virtual void ShootSuccess() override;
};
