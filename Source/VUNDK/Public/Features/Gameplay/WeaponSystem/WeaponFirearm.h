// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Shooter/Shooter.h"
#include "WeaponFirearm.generated.h"

UCLASS()
class VUNDK_API AWeaponFirearm : public AWeaponBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UShootBarrel* ShootBarrel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ShootBarrelSocketName = "Barrel";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EShootType WeaponShootType;
	
public:
	AWeaponFirearm();
	
	virtual void Init(APawn* InOwner) override;

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
	void SetWeaponAmmoRemaining(const int32 NewAmmoRemaining) const;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType);

	UFUNCTION(BlueprintCallable)
	void SetWeaponMaxSpread(const float NewSpread) const;
	
	UFUNCTION(BlueprintCallable)
	int32 Reload(int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void ReloadAllMagazine();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnReload();
	
	virtual bool DeployWeaponAttack_Implementation() override;
};
