// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GHMovementModeBase.h"
#include "GHTargetTowardSelf.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Target Toward Self"))
class GVUEDK_API UGHTargetTowardSelf : public UGHMovementModeBase
{
	GENERATED_BODY()

public:
	// virtual void StartHooking() override;
	// virtual void StopHooking() override;
	// virtual bool TickMode(float DeltaTime) override;

	// - target speed + speed curve
	// - direction
	// - start delay
	// - start location
	// - end location
	
};
