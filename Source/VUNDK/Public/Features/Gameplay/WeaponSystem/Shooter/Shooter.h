// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootBarrel.h"
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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FShootData ShootData;
	
public:
	UShooter();

	UFUNCTION(BlueprintCallable)
	void Init(const UShootBarrel* InShootBarrel);

	UFUNCTION(BlueprintCallable)
	void SetOwner(APawn* InOwner) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ResetRecoil() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 Refill(const int32 Ammo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void RefillAllMagazine() const;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool Check() const;
};
