// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/Data/ShootData.h"
#include "Features/Gameplay/WeaponSystem/Interfaces/ShooterBehaviour.h"
#include "UObject/Object.h"
#include "ShooterBehaviourBase.generated.h"

class UShooter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnShootSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnShootFail
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRefill
);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VUNDK_API UShooterBehaviourBase : public UObject, public IShooterBehaviour
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnShootSuccess OnBehaviourShootSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnShootFail OnBehaviourShootFail;
	UPROPERTY(BlueprintAssignable)
	FOnRefill OnBehaviourRefill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHasInfiniteAmmo = false;

protected:
	UPROPERTY(BlueprintReadOnly)
	UShooter* Shooter;

private:
	FShootData ShootData;
	int32 CurrentAmmo;
	bool bIsInCooldown;

public:
	virtual void Init(UShooter* InShooter, const FShootData InShootData);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot() override;

	virtual void ShootSuccess();

	virtual void ShootFail();
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual void RefillAllMagazine() override;

	UFUNCTION(BlueprintPure)
	virtual int32 GetCurrentAmmo() const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRange() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	bool CanShoot() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess();

	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail();

	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();

private:
	void SetCurrentAmmo(const int32 NewAmmoValue);

	void ModifyCurrentAmmo(const int32 AmmoValue);
	
	void StartShootCooldown();

	void EndShootCooldown();
	
	bool Check() const;
};
