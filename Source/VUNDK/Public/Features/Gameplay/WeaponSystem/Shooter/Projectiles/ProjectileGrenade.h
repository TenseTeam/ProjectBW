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
	float ExplosionRadius = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToExplosion = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "if true, damage not scaled based on distance from Origin."))
	bool bDoFullDamage = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> ExplosionPreventionChannel = ECC_Visibility;

public:
	AProjectileGrenade();

protected:
	virtual void InitVelocityAndLifeSpan_Implementation(float InRange, float InSpeed, const FVector& InDirection) override;

	UFUNCTION(BlueprintCallable)
	void Explode();

	virtual void OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;
		
	virtual void OnProjectileLifeSpanEnd_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnExplosion();
};
