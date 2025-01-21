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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnShootSuccess,
	UShootPoint*, ShootPoint
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnShootFail
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnRefill,
	int32, CurrentAmmo
);

constexpr float RecoilStrengthMultiplier = 100.f;
constexpr float RangeMultiplier = 100.f;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VUNDK_API UShooterBehaviourBase : public UObject, public IShooterBehaviour
{
	GENERATED_BODY()

	friend class UShooter;

public:
	UPROPERTY(BlueprintAssignable)
	FOnShootSuccess OnBehaviourShootSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnShootFail OnBehaviourShootFail;
	UPROPERTY(BlueprintAssignable)
	FOnRefill OnBehaviourRefill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> SightTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasInfiniteAmmo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraTargetLocation = true;

protected:
	UPROPERTY(BlueprintReadOnly)
	UShooter* Shooter;
	UPROPERTY(BlueprintReadOnly)
	APawn* Owner;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f, AllowPrivateAccess = true))
	int32 AmmoToConsumePerShot = 1;
	UPROPERTY()
	TArray<UShootPoint*> ShootPoints;
	int32 CurrentAmmo;
	int32 ShotsFired;
	FShootData ShootData;
	bool bIsInCooldown;
	int32 CurrentShootPointIndex;
	bool bIsBehaviourActive;
	FRotator ImpulseRecoil;
	float CooldownRemaining;
	float RecoilRemaining;

public:
	virtual void Init(UShooter* InShooter, const FShootData InShootData, const TArray<UShootPoint*> InShootPoints);

	void SetOwner(APawn* InOwner);

	UFUNCTION(BlueprintCallable)
	void EnableBehaviour();

	UFUNCTION(BlueprintCallable)
	void DisableBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual bool Shoot(const EShootType ShootType = EShootType::Simultaneous) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ResetRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual int32 Refill(const int32 Ammo) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual void RefillAllMagazine() override;

	UFUNCTION(BlueprintCallable)
	void SetShootParams(const float NewDamage, const float NewFireRate, const float NewRange, const int32 NewMagSize, const int32 NewRecoilStrength);

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetFireRate(const float NewFireRate);

	UFUNCTION(BlueprintCallable)
	void SetRange(const float NewRange);

	UFUNCTION(BlueprintCallable)
	void SetMagSize(const int32 NewMagSize);

	UFUNCTION(BlueprintCallable)
	void SetRecoilStrength(float NewRecoilStrength);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 NewAmmo);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetFireRate() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRange() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetMagSize() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetRecoilStrength() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int32 GetAmmoToConsume() const;
	
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

protected:
	void ShootSuccess(UShootPoint* ShootPoint);

	void ShootFail();

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnTickBehaviour(const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourEnabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnBehaviourDisabled();

	UFUNCTION(BlueprintNativeEvent)
	void OnDeployShoot(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootSuccess(UShootPoint* ShootPoint, const FVector& TargetLocation, const FVector& DirectionToTarget) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnShootFail();

	UFUNCTION(BlueprintNativeEvent)
	void OnRefill();

	UFUNCTION(BlueprintNativeEvent)
	FVector CalculateShooterTargetLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShooterTargetLocation() const;

	UFUNCTION(BlueprintCallable)
	bool TryGetCameraPoints(FVector& OutStartPoint, FVector& OutEndPoint, FVector& OutHitPoint) const;

	UFUNCTION(BlueprintPure)
	bool IsInLineOfSight(const FVector& StartPoint, const FVector& TargetPoint, const float Tolerance = 50.0f) const;

	virtual bool Check() const;

private:
	void TickBehaviour(float DeltaTime);

	bool HandleSimultaneousShoot();

	bool HandleSequentialShoot();

	bool HasEnoughAmmoToShoot(const int32 DesiredAmmoToConsume) const;

	int32 NextShootPointIndex();

	void ModifyCurrentAmmo(const int32 AmmoValue);

	void ApplyRecoilImpulse();

	void ProcessRecoilImpulseRotation(float DeltaTime);

	void StartShootCooldown();

	void ProcessCooldown(float DeltaTime);

	void EndShootCooldown();
};
