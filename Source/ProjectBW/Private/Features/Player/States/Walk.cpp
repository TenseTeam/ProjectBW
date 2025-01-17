// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Walk.h"

void UWalk::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	
	Super::Enter(Context);
	if (Character->WantRunning() && Character->CanRun() && !Character->IsAiming() && !Character->IsShooting())
	{
		Character->ChangeMotionState(2);
		return;
	}
	Character->MotionState = ECharacterState::Walking;

	Character->OnStopAiming.AddDynamic(this, &UWalk::CheckWantRun);
	Character->OnStopShooting.AddDynamic(this, &UWalk::CheckWantRun);
}

void UWalk::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	
	Super::Update(Context, DeltaTime);
	if (Character->WantRunning() && Character->CanRun() && !Character->IsAiming() && !Character->IsShooting())
	{
		Character->ChangeMotionState(2);
		return;
	}
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		Character->ChangeMotionState(0);
	}
}

void UWalk::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->OnStopAiming.RemoveDynamic(this, &UWalk::CheckWantRun);
	Character->OnStopShooting.RemoveDynamic(this, &UWalk::CheckWantRun);
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
		if (Value.Get<bool>()) //important to check if the value is true
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

	if (InputAction == EInputActionType::Dodge)
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

void UWalk::CheckWantRun()
{
	if (Character->IsAiming() || Character->IsShooting())
    {
        return;
    }
	
	if (Character->WantRunning() && Character->CanRun())
	{
		Character->ChangeMotionState(2);
	}
}
