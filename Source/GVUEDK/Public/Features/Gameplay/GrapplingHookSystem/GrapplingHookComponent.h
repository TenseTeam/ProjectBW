// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrabPoint.h"
#include "GrapplingHookComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopHooking);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UGrapplingHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FStartHooking OnStartHooking;
	UPROPERTY(BlueprintAssignable)
	FHooking OnHooking;
	UPROPERTY(BlueprintAssignable)
	FStopHooking OnStopHooking;

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
	bool bShowDebug;

	TSet<IGrabPoint*> InRangeGrabPoints;
	IGrabPoint* TargetGrabPoint;

	FVector StartHookLocation;
	FVector EndHookLocation;
	FVector StartHookDirection;
	float TotalHookDistance;
	float ElapsedTime;
	
	bool bTargetAcquired;
	bool bIsHooking;

public:
	UGrapplingHookComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool IsTargetAcquired() const { return bTargetAcquired; }
	float GetMaxDistance() const { return MaxDistance; }
	float GetMinDistance() const { return MinDistance; }
	float GetTotalHookDistance() const { return bIsHooking ? TotalHookDistance : 0.f; }
	float GetStartDelay() const { return StartDelay; }
	FVector GetStartLocation() const { return bIsHooking ? StartHookLocation : FVector::ZeroVector; }
	FVector GetEndLocation() const { return bIsHooking ? EndHookLocation : FVector::ZeroVector; }
	FVector GetStartDirection() const { return bIsHooking ? StartHookDirection : FVector::ZeroVector; }

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
	bool PerformSphereTrace(TArray<FHitResult>& HitResults) const;
	IGrabPoint* GetNearestGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const;
	float GetElapsedNormalizedDistance() const;
	
};


