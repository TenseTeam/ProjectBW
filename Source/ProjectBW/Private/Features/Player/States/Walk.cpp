// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Walk.h"

void UWalk::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	
	Super::Enter(Context);
	if (Character->IsRunning())
	{
		Character->ChangeState(2);
		return;
	}
	Character->CharacterState = ECharacterState::Walking;
}

void UWalk::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	
	Super::Update(Context, DeltaTime);
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		Character->ChangeState(0);
	}
}

void UWalk::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	
	Super::HandleInput(Context, InputAction, Value);

	if (InputAction == EInputActionType::Walk)
	{
		const FVector MoveVector = Value.Get<FVector>();
		Character->Move(MoveVector);
		return;
	}

	if (InputAction == EInputActionType::Run)
	{
		Character->ChangeState(2);
		return;
	}

	if (InputAction == EInputActionType::Jump)
	{
		Character->ChangeState(3);
		return;
	}

	if (InputAction == EInputActionType::Dodge /*&& !Character->GetCharacterMovement()->IsFalling() */)
	{
		Character->ChangeState(4);
		return;
	}

	if (InputAction == EInputActionType::Hook)
	{
		Character->ChangeState(5);
		return;
	}
}
