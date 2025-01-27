// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GHSearchModeBase.h"
#include "GHSphereTraceSearch.generated.h"

/**
 * 
 */
UCLASS()
class GVUEDK_API UGHSphereTraceSearch : public UGHSearchModeBase
{
	GENERATED_BODY()

public:
	UGHSphereTraceSearch();

	virtual void Initialize(UGrapplingHookComponent* InGrapplingHookComponent) override;
	virtual bool TickMode(float DeltaTime) override;
	
	virtual float GetMaxDistance() const override { return MaxDistance; }
	virtual float GetMinDistance() const override { return MinDistance; }

private:
	bool LookForGrabPoints(TSet<IGrabPoint*>& OutGrabPoints) const;
	IGrabPoint* GetNearestGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const;
	bool PerformSphereTrace(TArray<FHitResult>& HitResults) const;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float MaxDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float MinDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;
	
	TSet<IGrabPoint*> InRangeGrabPoints;
	IGrabPoint* LastTargetGrabPoint;
	
};
