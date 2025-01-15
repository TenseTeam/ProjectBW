// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Behaviours/ShooterBehaviourBase.h"
#include "Components/ActorComponent.h"
#include "Data/ShootData.h"
#include "Shooter.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UShooter : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UShooterBehaviourBase* ShooterBehaviour;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FShootData ShootData;
	UPROPERTY(BlueprintReadOnly)
	TArray<UShootPoint*> ShootPoints;

public:
	UShooter();

	UFUNCTION(BlueprintCallable)
	void Init(const TArray<UShootPoint*> InShootPoints);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot(const EShootType ShootType = EShootType::Simultaneous) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(const int32 Ammo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void RefillAllMagazine() const;

	UFUNCTION(BlueprintCallable)
	void SetShooterDamage(const float NewDamage) const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

private:
	bool Check() const;
};
