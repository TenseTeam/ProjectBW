// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "GrenadeProjectile.generated.h"

UCLASS()
class VUNDK_API AGrenadeProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AGrenadeProjectile();

	virtual void OnPooledActorBeginPlay_Implementation() override;

	virtual void OnPooledActorEndPlay_Implementation() override;

protected:
	virtual void OnProjectileLifeSpanEnd_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnExplosion();
};
