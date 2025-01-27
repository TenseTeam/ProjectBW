// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/Shooter/ShootBarrel.h"
#include "Features/Gameplay/WeaponSystem/Shooter/ShootPoint.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Data/ShootData.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Data/ShootFailReason.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Interfaces/ShooterBehaviour.h"
#include "UObject/Object.h"
#include "ShooterBehaviourBase.generated.h"

class UShooter;
enum class EShootType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBehaviourShootSuccess,
	UShootBarrel*, ShootBarrel,
	int32, ShotIndex
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnBehaviourShootFail
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBehaviourRefill,
	int32, CurrentAmmo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCurrentAmmoChanged,
	int32, CurrentAmmo,
	int32, MagSize
);

constexpr float RecoilStrengthMultiplier = 100.f;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VUNDK_API UShooterBehaviourBase : public UObject, public IShooterBehaviour
{
	GENERATED_BODY()

	friend class UShooter;

public:
	UPROPERTY(BlueprintAssignable)
	FOnBehaviourShootSuccess OnBehaviourShootSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnBehaviourShootFail OnBehaviourShootFail;
	UPROPERTY(BlueprintAssignable)
	FOnBehaviourRefill OnBehaviourRefill;
	UPROPERTY(BlueprintAssignable)
	FOnCurrentAmmoChanged OnCurrentAmmoChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> SightTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasInfiniteAmmo = false;

protected:
	UPROPERTY(BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(BlueprintReadOnly)
	APawn* Owner;
	UPROPERTY(BlueprintReadOnly)
	TArray<UShootPoint*> ShootPoints;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, UIMin = 0, AllowPrivateAccess = true))
	int32 AmmoToConsumePerShot = 1;
	UPROPERTY()
	UShootBarrel* ShootBarrel;
	FShootData ShootData;
	int32 CurrentAmmo;
	int32 ShotsFired;
	int32 CurrentShootPointIndex;
	bool bIsInCooldown;
	bool bIsBehaviourActive;
	float RecoilRemaining;
	float CooldownRemaining;
	FRotator ImpulseRecoil;
	FRotator BaselineControlRotation;

public:
	virtual void Init(UShooter* InShooter, const FShootData InShootData, UShootBarrel* InShootBarrel);

	void SetOwner(APawn* InOwner);

	UFUNCTION(BlueprintCallable)
	void EnableBehaviour();

	UFUNCTION(BlueprintCallable)
	void DisableBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot() override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ResetRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual void RefillAllMagazine() override;

	UFUNCTION(BlueprintCallable)
	void SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength, const float NewSpread);

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetFireRate(const float NewFireRate);

	UFUNCTION(BlueprintCallable)
	void SetMaxRange(const float NewRange);

	UFUNCTION(BlueprintCallable)
	void SetMagSize(const int32 NewMagSize);

	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(float NewRecoilStrength);

	UFUNCTION(BlueprintCallable)
	void SetMaxSpread(const float NewMaxSpread);

	UFUNCTION(BlueprintCallable)
	void SetShootType(const EShootType NewShootType);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo);

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentAmmo(const int32 AmmoValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetMaxRange() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRecoilStrength() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetMaxSpread() const;

	UFUNCTION(BlueprintPure)
	EShootType GetShootType() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetAmmoToConsume() const;

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

protected:
	virtual void HandleShoot();

	void ShootFromShootPoint(UShootPoint* ShootPoint) const;

	void ShootSuccess();

	void ShootFail(const EShootFailReason FailReason);

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnTickBehaviour(const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourEnabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourDisabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployShoot(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootFromShootPoint(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(const UShootBarrel* OutShootBarrel);

	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail(const EShootFailReason FailReason);

	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();

	/**
	 * Gets the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point.
	 * @return The location of the target the shooter is aiming at.
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Get the location of the target the shooter is aiming at. By default, it returns the location of the camera hit point."))
	FVector GetShooterTargetLocation() const;

	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint, FRotator& OutRotation, FVector StartPointOffset = FVector::ZeroVector) const;

	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 25.0f) const;

	virtual bool Check() const;

private:
	void TickBehaviour(float DeltaTime);

	void HandleSimultaneousShoot();

	void HandleSequentialShoot();

	int32 NextShootPointIndex();

	bool TryConsumeAmmoForShoot();

	bool HasEnoughAmmoToShoot() const;
	
	void ApplyRecoilImpulse();

	void ProcessRecoilImpulseRotation(float DeltaTime);

	void StartShootCooldown();

	void ProcessCooldown(float DeltaTime);

	void EndShootCooldown();

	float GenerateSpread() const;
};
