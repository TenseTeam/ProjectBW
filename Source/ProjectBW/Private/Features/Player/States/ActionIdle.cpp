// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/ActionIdle.h"

void UActionIdle::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	Character->ActionState = ECharacterState::None;

	Character->OnStopRunning.AddDynamic(this, &UActionIdle::CheckWantShootOrAim);
	Character->OnStopDodging.AddDynamic(this, &UActionIdle::CheckWantShootOrAim);
	Character->OnStopHook.AddDynamic(this, &UActionIdle::CheckWantShootOrAim);
}

void UActionIdle::Exit(AActor* Context)
{
	Super::Exit(Context);

	Character->OnStopRunning.RemoveDynamic(this, &UActionIdle::CheckWantShootOrAim);
	Character->OnStopDodging.RemoveDynamic(this, &UActionIdle::CheckWantShootOrAim);
	Character->OnStopHook.RemoveDynamic(this, &UActionIdle::CheckWantShootOrAim);
}

void UActionIdle::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);

	if (InputAction == EInputActionType::Shoot || InputAction == EInputActionType::Aim)
	{
		//Character->OnStartShooting.Broadcast();
		if (Value.Get<bool>()) Character->ChangeActionState(1); // shoot
		return;
	}
}

void UActionIdle::CheckWantShootOrAim()
{
    if (Character->WantShooting() || Character->WantAiming())
    {
        Character->ChangeActionState(1); // shoot
    }
}
