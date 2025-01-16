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
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugTraceLines = false;
#endif
	
protected:
	virtual void OnShootSuccess_Implementation(UShootPoint* ShootPoint, const FVector& ShooterTargetLocation, const FVector& ShootPointDirectionToTarget) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnLineTraceDamage(const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const;
	
private:
	void TraceFromCamera(const UWorld* World, const UShootPoint* ShootPoint) const;

	void TraceFromShootPoint(const UWorld* World, const UShootPoint* ShootPoint) const;

	void LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const;
};
