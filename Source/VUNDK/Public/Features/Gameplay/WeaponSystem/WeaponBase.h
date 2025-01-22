// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackFail
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponAttackSuccess OnWeaponAttackSuccessEvent;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponAttackFail OnWeaponAttackFailEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponData WeaponData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComponent;

public:
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	virtual void Init(APawn* InOwner);

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponDamage(const float NewDamage);

	UFUNCTION(BlueprintCallable)
	bool WeaponAttack();

	UFUNCTION(BlueprintPure)
	FWeaponData GetWeaponData() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool DeployWeaponAttack();

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackSuccess();

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackFail();
};
