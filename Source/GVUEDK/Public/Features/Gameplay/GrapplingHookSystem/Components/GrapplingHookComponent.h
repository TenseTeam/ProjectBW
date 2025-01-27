// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Interfaces/GrabPoint.h"
#include "GrapplingHookComponent.generated.h"

class UGHSearchModeBase;
class UGHMovementModeBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHookMotionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptHooking);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UGrapplingHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FStartHooking OnStartHooking;
	UPROPERTY(BlueprintAssignable)
	FHookMotionStarted OnHookMotionStarted;
	UPROPERTY(BlueprintAssignable)
	FHooking OnHooking;
	UPROPERTY(BlueprintAssignable)
	FStopHooking OnStopHooking;
	UPROPERTY(BlueprintAssignable)
	FInterruptHooking OnInterruptHooking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UGHMovementModeBase*> MovementModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UGHSearchModeBase*> SearchModes;

	UFUNCTION(BlueprintCallable)
	void ChangeMovementMode(int32 ModeIndex);
	UFUNCTION(BlueprintCallable)
	void ChangeSearchMode(int32 ModeIndex);

	ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }
	IGrabPoint* GetTargetGrabPoint() const { return TargetGrabPoint; }
	void SetTargetGrabPoint(IGrabPoint* GrabPoint) { TargetGrabPoint = GrabPoint; bTargetAcquired = TargetGrabPoint != nullptr; }
	bool IsTargetAcquired() const { return bTargetAcquired; } 

private:
	UPROPERTY()
	UGHMovementModeBase* CurrentMovementMode;
	UPROPERTY()
	UGHSearchModeBase* CurrentSearchMode;

	// Returns true if the modes are initialized successfully
	bool InitializeModes();
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float MaxDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float MinDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SpeedCurve;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "Used only if no SpeedCurve provided"))
	float LinearSpeed;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float StartDelay;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bOrientRotationToMovement;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bApplyMomentumDuringHookThrow;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	UPROPERTY()
	ACharacter* OwnerCharacter;
	
	TSet<IGrabPoint*> InRangeGrabPoints;
	IGrabPoint* TargetGrabPoint;
	IGrabPoint* LastTargetGrabPoint;

	FVector StartHookLocation;
	FVector EndHookLocation;
	FVector StartHookDirection;
	float TotalHookDistance;
	float ElapsedTime;
	
	bool bTargetAcquired;
	bool bIsHooking;
	bool bMotionDataCalculated;
	bool bInitialized;

public:
	UGrapplingHookComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
	float GetMaxDistance() const;
	float GetMinDistance() const;
	float GetTotalHookDistance() const;
	
	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const;
	UFUNCTION(BlueprintCallable)
	float GetStartDelay() const;
	UFUNCTION(BlueprintCallable)
	FVector GetStartLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetLandingPointLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetStartDirection() const;
	UFUNCTION(BlueprintCallable)
	FVector GetTargetGrabPointLocation() const { return bTargetAcquired ? TargetGrabPoint->GetLocation() : FVector::ZeroVector; }

	UFUNCTION(BlueprintCallable)
	void StartHooking();
	UFUNCTION(BlueprintCallable)
	void StopHooking();

protected:
	virtual void PerformHooking(float DeltaTime);
	virtual float GetSpeed() const;
	virtual FVector GetDirection() const;

private:
	bool LookForGrabPoints(TSet<IGrabPoint*>& OutGrabPoints) const;
	IGrabPoint* GetNearestGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const;
	float GetElapsedNormalizedDistance() const;
	void OrientRotationToMovement(float DeltaTime) const;
	// Returns true if the motion data is calculated successfully
	bool CalculateMotionData();
	bool PerformSphereTrace(TArray<FHitResult>& HitResults) const;
	
};


