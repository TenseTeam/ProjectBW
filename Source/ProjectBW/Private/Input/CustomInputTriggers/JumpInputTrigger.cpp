// Copyright Villains, Inc. All Rights Reserved.


#include "Input/CustomInputTriggers/JumpInputTrigger.h"

ETriggerState UJumpInputTrigger::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                            FInputActionValue ModifiedValue, float DeltaTime)
{
	ETriggerState TriggerState = Super::UpdateState_Implementation(PlayerInput, ModifiedValue, DeltaTime);
	
	if (TriggerState == ETriggerState::Triggered)
	{
		InputElapsedTime += DeltaTime;
		if (InputElapsedTime >= InputElapsedTimeLimit)
		{
			return ETriggerState::None;
		}
		return ETriggerState::Triggered;
	}
	InputElapsedTime = 0;
	return ETriggerState::None;
}
