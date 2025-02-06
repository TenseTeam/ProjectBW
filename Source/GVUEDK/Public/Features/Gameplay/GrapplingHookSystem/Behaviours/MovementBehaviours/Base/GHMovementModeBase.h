// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Behaviours/Base/GrapplingHookModeBase.h"
#include "GHMovementModeBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class GVUEDK_API UGHMovementModeBase : public UGrapplingHookModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void StartHooking(){};
	UFUNCTION(BlueprintCallable)
	virtual void StopHooking(){};

	virtual FVector GetStartDirection() const { return FVector::ZeroVector; }
	virtual FVector GetLandingPointLocation() const { return FVector::ZeroVector; }
	virtual FVector GetStartLocation() const { return FVector::ZeroVector; }
	virtual float GetElapsedTime() const { return ElapsedTime; }
	virtual float GetStartDelay() const { return StartDelay; }
	virtual float GetTotalHookDistance() const { return 0.f; }

protected:
	virtual bool TryCalculateMotionData() { return true; }

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float StartDelay;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bApplyMomentumDuringHookThrow;

	float CachedGravityScale;
	float ElapsedTime;
	
	bool bIsHooking;
	bool bMotionDataCalculated;
};
