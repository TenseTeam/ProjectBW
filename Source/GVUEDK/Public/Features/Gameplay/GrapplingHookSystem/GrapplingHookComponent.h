// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrabPoint.h"
#include "GrapplingHookComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UGrapplingHookComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float MaxDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float MinDistance;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	TSet<IGrabPoint*> InRangeGrabPoints;
	IGrabPoint* TargetGrabPoint;

	bool bTargetAcquired;

public:
	UGrapplingHookComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	float GetMaxDistance() const { return MaxDistance; }
	float GetMinDistance() const { return MinDistance; }

private:
	bool LookForGrabPoints(TSet<IGrabPoint*>& OutGrabPoints) const;
	bool PerformSphereTrace(TArray<FHitResult>& HitResults) const;
	IGrabPoint* GetNearestGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const;
};
