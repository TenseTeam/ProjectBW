// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/MotionStates/MotionIdle.h"

void UMotionIdle::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	
	Character->MotionState = ECharacterState::Standing;
}

void UMotionIdle::Update(AActor* Context, float deltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, deltaTime);
}

void UMotionIdle::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);

	if (InputAction == EInputActionType::Walk)
	{
		Character->ChangeMotionState(1);
		return;
	}

	if (InputAction == EInputActionType::Run)
	{
		if (Value.Get<bool>() && !Controller->GetMoveInputValue().IsNearlyZero()) //important to check if the Value.Get is true
		{
			Character->ChangeMotionState(2);
			return;
		}
	}

	if (InputAction == EInputActionType::Jump)
	{
		Character->ChangeMotionState(3);
		return;
	}

	if (InputAction == EInputActionType::Dodge /*&& !Character->GetCharacterMovement()->IsFalling() */)
	{
		Character->ChangeMotionState(4);
		return;
	}

	if (InputAction == EInputActionType::Hook)
	{
		Character->ChangeMotionState(5);
		return;
	}
}
