// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Run.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void URun::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	Character->CharacterState = ECharacterState::Running;
	Character->SetIsRunning(true);
	
	DefaultAcceleration = Character->GetCharacterMovement()->MaxAcceleration;
	DefaultSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

	if (UCharacterData* charData = Character->Data)
	{
		Character->GetCharacterMovement()->MaxAcceleration = charData->RunAcceleration;
		Character->GetCharacterMovement()->MaxWalkSpeed = charData->RunSpeed;
	}
	else
	{
		FGvDebug::Warning("Character Has Invalid Data", true);
	}
	
}

void URun::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);

	if (!Character->IsRunning())
	{
		if (Controller->GetMoveInputValue().IsNearlyZero())
		{
			Character->ChangeState(0); //idle
		}
		else
		{
			Character->ChangeState(1); //walk
		}
	}
}

void URun::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	//Character->SetIsRunning(false); wrong do not need to set this
	Character->GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
	Character->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

}

void URun::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);

	if (InputAction == EInputActionType::Walk)
	{
		Character->Move(Value.Get<FVector>());
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
}
