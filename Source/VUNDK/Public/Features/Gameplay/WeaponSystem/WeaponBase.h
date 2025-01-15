// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttack
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponAttack OnWeaponAttackEvent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponData WeaponData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComponent;

public:
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	bool WeaponAttack();

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool OnWeaponAttack();
};
