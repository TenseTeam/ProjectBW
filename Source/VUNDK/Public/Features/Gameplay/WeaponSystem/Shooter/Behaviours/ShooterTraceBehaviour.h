// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBehaviourBase.h"
#include "ShooterTraceBehaviour.generated.h"

UCLASS()
class VUNDK_API UShooterTraceBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxPenetration = 1;
	
protected:
	virtual void OnShootSuccess_Implementation(const FVector& ShootPointLocation, const FVector& ShootPointDirection, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const override;

private:
	void TraceFromCamera(const UWorld* World, const FVector& InShootPointLocation) const;

	void TraceFromShootPoint(const UWorld* World, const FVector& InShootPointLocation, const FVector& ShootPointDirectionToTarget) const;

	void LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const;
};
