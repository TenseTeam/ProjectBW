// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Idle.h"

#include "ShaderCompiler.h"

void UIdle::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	Character->CharacterState = ECharacterState::Standing;
}

void UIdle::Update(AActor* Context, float deltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, deltaTime);

	//if(Character->GetCharacterMovement()->IsFalling())
	//{
	//	Character->ChangeState(jump);
	//{
}

void UIdle::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);

	if (InputAction == EInputActionType::Walk)
	{
		Character->ChangeState(1);
		return;
	}

	if (InputAction == EInputActionType::Run)
	{
		Character->ChangeState(2);
		return;
	}
	//
	// if (InputAction == EInputActionType::Run)
	// {
	// 	if (UActorStateBase* runState = Controller->GetState(4))
	// 	{
	// 		Controller->ChangeControllerState(runState);
	// 	}
	// 	return;
	// }
	//
	// if (InputAction == EInputActionType::Roll && !Character->GetCharacterMovement()->IsFalling())
	// {
	// 	if (UActorStateBase* rollState = Controller->GetState(2))
	// 	{
	// 		Controller->ChangeControllerState(rollState);
	// 	}
	// 	return;
	// }
	//
	// if (InputAction == EInputActionType::Jump)
	// {
	// 	if (UActorStateBase* jumpState = Controller->GetState(3))
	// 	{
	// 		Controller->ChangeControllerState(jumpState);
	// 	}
	// 	return;
	// }
	//
	// if (InputAction == EInputActionType::Shoot || InputAction == EInputActionType::Aim)
	// {
	// 	if (UActorStateBase* shootState = Controller->GetState(5))
	// 	{
	// 		Controller->ChangeControllerState(shootState);
	// 	}
	// 	return;
	// }
	//
	// if (InputAction == EInputActionType::ThrowHook)
	// {
	// 	if (UActorStateBase* throwHookState = Controller->GetState(6))
	// 	{
	// 		Controller->ChangeControllerState(throwHookState);
	// 	}
	// 	return;
	// }
}
