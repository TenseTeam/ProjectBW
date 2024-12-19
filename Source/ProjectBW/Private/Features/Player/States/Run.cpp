// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Run.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void URun::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	
	if (!IsValid(Character->Data))
	{
		bInitialized = false;
		FGvDebug::Warning(Context->GetName() + " Has Invalid Data", true);
		return;
	}
	bInitialized = true;
}

void URun::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	Character->MotionState = ECharacterState::Running;
	//Character->SetWantRunning(true);
	
	DefaultAcceleration = Character->GetCharacterMovement()->MaxAcceleration;
	DefaultSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

	Character->GetCharacterMovement()->MaxAcceleration = Character->Data->RunAcceleration;
	Character->GetCharacterMovement()->MaxWalkSpeed = Character->Data->RunSpeed;

	Character->OnStartAiming.AddDynamic(this, &URun::InterruptRun);
	Character->OnStartShooting.AddDynamic(this, &URun::InterruptRun);
	Character->OnStartRunning.Broadcast();
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
		InterruptRun();
	}
}

void URun::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	
	Character->GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
	Character->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	Character->OnStartAiming.RemoveDynamic(this, &URun::InterruptRun);
	Character->OnStartShooting.RemoveDynamic(this, &URun::InterruptRun);
	Character->OnStopRunning.Broadcast();

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

void URun::InterruptRun()
{
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		Character->ChangeMotionState(0); //idle
	}
	else
	{
		Character->ChangeMotionState(1); //walk
	}
}

// void URun::PauseRun()
// {
// 	bPaused = true;
// 	Character->MotionState = ECharacterState::Walking;
// 	Character->GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
// 	Character->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
// }
//
// void URun::UnpauseRun()
// {
// 	bPaused = false;
// 	Character->MotionState = ECharacterState::Running;
// 	Character->GetCharacterMovement()->MaxAcceleration = Character->Data->RunAcceleration;
// 	Character->GetCharacterMovement()->MaxWalkSpeed = Character->Data->RunSpeed;
// }
//
// void URun::OnStartShooting()
// {
// 	bIsShooting = true;
// 	PauseRun();
// }
//
// void URun::OnStartAiming()
// {
// 	bIsAiming = true;
// 	PauseRun();
// }
//
// void URun::OnStopShooting()
// {
// 	bIsShooting = false;
// 	if (!bIsAiming)
// 	{
// 		UnpauseRun();
// 	}
// }
//
// void URun::OnStopAiming()
// {
// 	bIsAiming = false;
// 	if (!bIsShooting)
// 	{
// 		UnpauseRun();
// 	}
// }
