// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBehaviourBase.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Data/ShootTraceMode.h"
#include "ShooterTraceBehaviour.generated.h"

UCLASS()
class VUNDK_API UShooterTraceBehaviour : public UShooterBehaviourBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxPenetration = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECollisionChannel::ECC_Visibility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EShootTraceMode> ShootTraceMode = EShootTraceMode::CameraSightTrace;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugTraceLines = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bDrawDebugTraceLines", EditConditionHides))
	float DebugTraceLineDuration = 5.f;
#endif

protected:
	virtual void OnDeployShoot_Implementation(UShootPoint* ShootPoint, const bool bIsUsingCameraHitTargetLocation, const FVector& TargetLocation, const FVector& DirectionToTarget) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnHitResults(const FVector& HitLocation, const TArray<FHitResult>& TraceHitResults, const TArray<FHitResult>& DamageHitResults) const;

private:
	void CameraSightTrace(const UWorld* World, const UShootPoint* ShootPoint) const;
	
	void ShootPointTrace(const UWorld* World, const UShootPoint* ShootPoint) const;

	void LineTraceDamage(const UWorld* World, const FVector& TraceStartPoint, const FVector& TraceEndPoint) const;
};
