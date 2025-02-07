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
	/**
     * Not really used, here just for possible future additionals features. 
     */
	virtual bool TryCalculateMotionData() override;

private:
	float GetHookSpeed() const;

	/**
     * Gets the normalized distance of the hook from the thrower.
     */
	float GetCurrentHookExtensionNormalized() const;
	
	FTransform GetOwnerTransform() const;

	/**
     * Gets the location of the socket of the character mesh where the hook should start from.
     */
	FVector GetHookStartLocationSocket() const;

	/**
     * Gets the location of the socket of the character mesh where the hook should stop.
     */
	FVector GetHookEndLocationSocket() const;

	/**
     * Gets the direction of the hook motion during the way out motion by performing a trace from the thrower to the pointed location.
     */
	FVector GetAimDirection() const;

	/**
     * @return true if the hook should start the way back motion.
     */
	bool IsMaxExtensionReached() const;

	/**
     * @return true if the hook should stop the way back motion.
     */
	bool IsMinExtensionReached(const float DeltaTime) const;

	/**
     * Perform the motion of the hook.
     * @return true if the hook should stop the way back motion.
     */
	bool PerformMotion(float DeltaTime);
	
	void OrientGrabbedActorRotationToMovement() const;

private:
	/**
	 * Max distance reachable by the hook from the thrower.
	 */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxHookExtension;
	/**
	 * Curve for modifying the speed of the hook during its motion,
	 * get in input the normalized distance of the hook from the thrower, \n
	 * Vector.X = speed for the way out motion; \n
	 * Vector.Y = speed for the way back motion with target; \n
	 * Vector.Z = speed for the way back motion without target. \n
	 */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCurveVector* HookSpeedCurve;
	
	/**
     * Linear speed for the hook motion, used only if no SpeedCurve provided.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "Used only if no SpeedCurve provided"))
	float HookLinearSpeed;
	
	/**
     * Name of the socket of the character mesh where the hook should start from.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName HookStartLocationSocket;

	/**
     * Name of the socket of the character mesh where the hook should stop.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName HookEndLocationSocket;

	/**
     * Collision channel for the aim trace performed to find the direction of the hook motion during the way out motion.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> AimTraceChannel;

	/**
     * if true the hook can grab targets on the way back to the thrower.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "if true the hook can grab targets on the way back to the player"))
	bool bCanGrabTargetOnHookReduction;

	/**
     * if true the grabbed actor rotation will be oriented to face the thrower during the way back motion.
     */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bOrientGrabbedActorRotationToMovement;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	/**
     * Location where the hook should invert its motion direction after grabbing a target.
     */
	FVector TargetGrabbedLocation;

	/**
     * Direction of the hook motion during the way out motion.
     */
	FVector ExtensionDirection;

	/**
     * true if the hook should start the way back motion.
     */
	bool bMaxExtensionReached;
	
};
