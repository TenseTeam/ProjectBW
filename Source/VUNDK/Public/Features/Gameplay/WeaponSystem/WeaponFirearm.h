// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Data/WeaponFirearmData.h"
#include "Shooter/Shooter.h"
#include "WeaponFirearm.generated.h"

UCLASS()
class VUNDK_API AWeaponFirearm : public AWeaponBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponFirearmData FirearmData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShootBarrel* ShootBarrel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ShootBarrelSocketName = "Barrel";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EShootType WeaponShootType;

private:
	bool bIsAimingDownSight;
	float DefaultMaxSpread;
	float AdsMaxSpread;
	float DefaultRecoilStrength;
	float AdsRecoilStrength;
	
public:
	AWeaponFirearm();

	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	UFUNCTION(BlueprintCallable)
	void ResetRecoil() const;
	
	virtual void SetWeaponDamage(const float NewDamage) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponFireRate(const float NewFireRate) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMagSize(const int32 NewMagSize) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxRange(const float NewRange) const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponRecoilStrength(const float NewRecoilStrength) const;
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo) const;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType);

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxSpread(const float NewSpread) const;

	UFUNCTION(BlueprintCallable)
	int32 ReloadWithAmmo(UAmmoTypeData* AmmoData, int32 Ammo);
	
	UFUNCTION(BlueprintCallable)
	int32 Reload(int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void ReloadAllMagazine();

	UFUNCTION(BlueprintCallable)
	void SetAimDownSight(bool bIsADSEnabled);

	UFUNCTION(BlueprintPure)
	bool IsAimingDownSight() const;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void EnableAimDownSight();

	virtual void DisableAimDownSight();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnReload();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnableAimDownSight();

	UFUNCTION(BlueprintNativeEvent)
	void OnDisableAimDownSight();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnCurrentAmmoChanged(int32 CurrentAmmo, int32 MagSize);
	
	virtual bool DeployWeaponAttack_Implementation() override;

private:
	void SetAimDownSightModifiers();
	
	void AttackBarrelToSocket() const;
};
