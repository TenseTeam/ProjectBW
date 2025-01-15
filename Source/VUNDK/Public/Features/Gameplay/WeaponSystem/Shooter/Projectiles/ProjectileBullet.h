// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class VUNDK_API AProjectileBullet : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectileBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
