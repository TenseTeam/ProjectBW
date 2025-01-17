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
	EShootType WeaponShootType;
	
public:
	AWeaponFirearm();

	virtual void SetWeaponDamage(const float NewDamage) override;

	void SetWeaponFireRate(const float NewFireRate) const;

	void SetWeaponMagSize(const int32 NewMagSize) const;

	void SetWeaponRange(const float NewRange) const;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponShootType(const EShootType NewShootType);
	
	UFUNCTION(BlueprintCallable)
	int32 Reload(int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void ReloadAllMagazine();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnReload();
	
	virtual bool OnWeaponAttack_Implementation() override;
};
