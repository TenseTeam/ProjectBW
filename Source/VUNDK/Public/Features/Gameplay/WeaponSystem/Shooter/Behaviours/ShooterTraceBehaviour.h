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
	virtual void OnShootSuccess_Implementation(const FVector& ShootPointLocation, const FVector& ShootPointDirection) override;
};
