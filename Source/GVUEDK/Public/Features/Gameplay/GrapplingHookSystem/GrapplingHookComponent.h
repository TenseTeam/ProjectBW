// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrabPoint.h"
#include "GrapplingHookComponent.generated.h"

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

private:
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
	

	bool IsTargetAcquired() const { return bTargetAcquired; }
	float GetMaxDistance() const { return MaxDistance; }
	float GetMinDistance() const { return MinDistance; }
	float GetTotalHookDistance() const { return bIsHooking ? TotalHookDistance : 0.f; }
	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const { return ElapsedTime; }
	UFUNCTION(BlueprintCallable)
	float GetStartDelay() const { return StartDelay; }
	UFUNCTION(BlueprintCallable)
	FVector GetStartLocation() const { return bIsHooking ? StartHookLocation : FVector::ZeroVector; }
	FVector GetLandingPointLocation() const { return bIsHooking ? EndHookLocation : FVector::ZeroVector; }
	UFUNCTION(BlueprintCallable)
	FVector GetStartDirection() const { return bIsHooking ? StartHookDirection : FVector::ZeroVector; }
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


