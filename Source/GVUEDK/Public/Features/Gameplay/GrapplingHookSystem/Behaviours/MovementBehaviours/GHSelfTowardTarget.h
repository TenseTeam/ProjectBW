// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GHMovementModeBase.h"
#include "GHSelfTowardTarget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Self Toward Target"))
class GVUEDK_API UGHSelfTowardTarget : public UGHMovementModeBase
{
	GENERATED_BODY()

public:
	UGHSelfTowardTarget();
	
	virtual void StartHooking() override;
	virtual void StopHooking() override;
	virtual bool TickMode(float DeltaTime) override;

	virtual FVector GetStartDirection() const override { return bIsHooking ? StartHookDirection : FVector::ZeroVector; }
	virtual FVector GetLandingPointLocation() const override { return bIsHooking ? EndHookLocation : FVector::ZeroVector; }
	virtual FVector GetStartLocation() const override { return bIsHooking ? StartHookLocation : FVector::ZeroVector; }
	virtual float GetElapsedTime() const override { return ElapsedTime; }
	virtual float GetStartDelay() const override { return StartDelay; }
	virtual float GetTotalHookDistance() const override { return TotalHookDistance; }

private:
	void PerformMotion(float DeltaTime);
	void OrientRotationToMovement(float DeltaTime);
	bool CalculateMotionData();
	float GetElapsedNormalizedDistance();
	float GetSpeed();

private:
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

	FVector StartHookLocation;
	FVector EndHookLocation;
	FVector StartHookDirection;
	
	float TotalHookDistance;
	float ElapsedTime;
	
	bool bIsHooking;
	bool bMotionDataCalculated;
};
