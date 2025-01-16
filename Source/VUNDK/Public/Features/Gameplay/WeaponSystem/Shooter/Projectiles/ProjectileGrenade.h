// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileGrenade.generated.h"

UCLASS()
class VUNDK_API AProjectileGrenade : public AProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionRadius = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> ExplosionPreventionChannel = ECC_Visibility;
	
public:
	AProjectileGrenade();
	
protected:
	void ApplyExplosionDamage();
	
	virtual void OnProjectileLifeSpanEnd_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnExplosion();
};
