// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/CustomInputTrigger.h"
#include "JumpInputTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UJumpInputTrigger : public UCustomInputTrigger
{
	GENERATED_BODY()

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
};
