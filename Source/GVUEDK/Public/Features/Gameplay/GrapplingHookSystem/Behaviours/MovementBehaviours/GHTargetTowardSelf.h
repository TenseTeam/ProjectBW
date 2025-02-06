// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GHMovementModeBase.h"
#include "GHTargetTowardSelf.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(meta = (DisplayName = "Target Toward Self"))
class GVUEDK_API UGHTargetTowardSelf : public UGHMovementModeBase
{
	GENERATED_BODY()

public:
	UGHTargetTowardSelf();
	virtual void StartHooking() override;
	virtual void StopHooking() override;
	virtual bool TickMode(float DeltaTime) override;

protected:
	virtual bool TryCalculateMotionData() override;

private:
	float GetHookSpeed() const;
	float GetCurrentHookExtensionNormalized() const;
	FTransform GetOwnerTransform() const;
	FVector GetHookStartLocationSocket() const;
	FVector GetHookEndLocationSocket() const;
	FVector GetAimDirection() const;
	bool IsMaxExtensionReached() const;
	bool IsMinExtensionReached(const float DeltaTime) const;
	bool PerformMotion(float DeltaTime);
	void OrientGrabbedActorRotationToMovement() const;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxHookExtension;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCurveVector* HookSpeedCurve;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "Used only if no SpeedCurve provided"))
	float HookLinearSpeed;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName HookStartLocationSocket;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName HookEndLocationSocket;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> AimTraceChannel;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "if true the hook can grab targets on the way back to the player"))
	bool bCanGrabTargetOnHookReduction;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bOrientGrabbedActorRotationToMovement;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	FVector TargetGrabbedLocation;
	FVector ExtensionDirection;
	bool bMaxExtensionReached;
	
};
