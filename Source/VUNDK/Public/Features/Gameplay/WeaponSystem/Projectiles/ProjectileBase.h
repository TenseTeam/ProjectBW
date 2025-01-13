// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Patterns/ObjectPool/PooledActorBase.h"
#include "ProjectileBase.generated.h"

UCLASS()
class VUNDK_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
