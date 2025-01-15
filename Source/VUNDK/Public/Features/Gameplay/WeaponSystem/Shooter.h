// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Behaviours/ShooterBehaviourBase.h"
#include "Components/ActorComponent.h"
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
	
public:
	UShooter();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(const int32 Ammo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void RefillAllMagazine() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

protected:
	virtual void BeginPlay() override;

private:
	bool Check() const;
};
