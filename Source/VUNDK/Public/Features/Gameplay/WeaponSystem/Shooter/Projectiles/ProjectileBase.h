// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Patterns/ObjectPool/PooledActorBase.h"
#include "ProjectileBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API AProjectileBase : public APooledActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	
private:
	UPROPERTY()
	AActor* ProjectileInstigator;
	float Damage;
	float LifeSpan;
	float HitRadius;
	TEnumAsByte<ECollisionChannel> DamageChannel;
	
public:
	AProjectileBase();

	void Init(AActor* InInstigator, const float InDamage, const float InLifeSpan, const float InHitRadius, TEnumAsByte<ECollisionChannel> InDamageChannel, const FVector& InVelocity);
	
	UFUNCTION(BlueprintCallable)
	void SetVelocity(const FVector NewVelocity) const;

	UFUNCTION(BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintPure)
	float GetHitRadius() const;

	UFUNCTION(BlueprintCallable)
	void RadialDamage() const;
	
	virtual void ClearPooledActor_Implementation() override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileLifeSpanEnd();
	
private:
	void SetProjectileLifeSpan(float InLifeSpan = 0.f);
};
