// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/Shooter/ShootPoint.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Data/ShootData.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Interfaces/ShooterBehaviour.h"
#include "UObject/Object.h"
#include "ShooterBehaviourBase.generated.h"

class UShooter;
enum class EShootType : uint8;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasInfiniteAmmo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraTargetLocation = true;

protected:
	UPROPERTY(BlueprintReadOnly)
	UShooter* Shooter;

private:
	UPROPERTY()
	TArray<UShootPoint*> ShootPoints;
	FShootData ShootData;
	int32 CurrentAmmo;
	bool bIsInCooldown;
	int32 CurrentShootPointIndex;

public:
	virtual void Init(UShooter* InShooter, const FShootData InShootData, const TArray<UShootPoint*> InShootPoints);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot(const EShootType ShootType = EShootType::Simultaneous) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual void RefillAllMagazine() override;

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	UFUNCTION(BlueprintPure)
	virtual int32 GetCurrentAmmo() const override;

	UFUNCTION(BlueprintPure)
	TEnumAsByte<ECollisionChannel> GetDamageChannel() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRange() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	UFUNCTION(BlueprintPure)
	UWorld* GetWorld() const;

protected:
	void ShootSuccess(const FVector& ShootPointLocation, const FVector& ShootPointDirection) const;

	void ShootFail();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(const FVector& ShootPointLocation, const FVector& ShootPointDirection, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const;

	/**
	 * @brief Called when the shoot is failed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail();

	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FVector CalculateShooterTargetLocation() const;
	
	UFUNCTION(BlueprintPure)
	FVector GetShooterTargetLocation() const;

	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint) const;

	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 1.0f) const;

private:
	bool HandleSimultaneousShoot();

	bool HandleSequentialShoot();

	bool CanShoot(const int32 DesiredAmmoToConsume) const;

	int32 NextShootPointIndex();

	void SetCurrentAmmo(const int32 NewAmmoValue);

	void ModifyCurrentAmmo(const int32 AmmoValue);

	void StartShootCooldown();

	void EndShootCooldown();

	bool Check() const;
};
