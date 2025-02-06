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
	virtual float GetTotalHookDistance() const override { return TotalHookDistance; }

protected:
	virtual bool TryCalculateMotionData() override;
	
private:
	void PerformMotion(float DeltaTime);
	void OrientRotationToMovement(float DeltaTime);
	float GetElapsedNormalizedDistance();
	float GetSpeed();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SpeedCurve;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "Used only if no SpeedCurve provided"))
	float LinearSpeed;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bOrientRotationToMovement;
	

	FVector StartHookLocation;
	FVector EndHookLocation;
	FVector StartHookDirection;
	
	float TotalHookDistance;
	
};
