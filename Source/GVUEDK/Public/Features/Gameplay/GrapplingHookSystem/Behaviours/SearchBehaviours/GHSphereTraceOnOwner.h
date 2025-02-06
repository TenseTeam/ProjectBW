// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GHSearchModeBase.h"
#include "GHSphereTraceOnOwner.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Sphere Trace On Owner"))
class GVUEDK_API UGHSphereTraceOnOwner : public UGHSearchModeBase
{
	GENERATED_BODY()

public:
	UGHSphereTraceOnOwner();

	virtual void Initialize(UGrapplingHookComponent* InGrapplingHookComponent) override;
	virtual bool TickMode(float DeltaTime) override;
	virtual void ExitMode() override;
	
	virtual float GetMaxDistance() const override { return MaxDistance; }
	virtual float GetMinDistance() const override { return MinDistance; }

private:
	bool LookForGrabPoints(TSet<IGrabPoint*>& OutGrabPoints) const;
	IGrabPoint* GetMostRelevantGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const;
	bool PerformSphereTrace(TArray<FHitResult>& HitResults) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> GrabPointsChannel;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float MaxDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float MinDistance;
	
	
	TSet<IGrabPoint*> InRangeGrabPoints;
	IGrabPoint* LastTargetGrabPoint;
	
};
