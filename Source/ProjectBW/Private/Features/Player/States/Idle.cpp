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
