// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/MotionStates/Jump.h"
#include "Features/Player/States/MotionStates/Hook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void UJump::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	if (!IsValid(Character->Data) || !IsValid(Character->Data->AscentGravityCurve) || !IsValid(
		Character->Data->DescentGravityCurve))
	{
		FGvDebug::Warning("Jump state has missing required data", true);
		bInitialized = false;
		return;
	}

	// register to the apex reached event
	ABWCharacter::OnNotifyApexEvent.AddDynamic(this, &UJump::OnJumpApexReached);
}

void UJump::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	// Check if character is going up or down or if the previous motion state was hook state, and set the correct jump state
	if (Character->GetVelocity().Z < 0 || IsValid(Cast<UHook>(Character->GetPreviousMotionState())))
	{
		Character->JumpState = EJumpState::JumpLoop;
	}
	else
	{
		Character->JumpState = EJumpState::JumpStart;
		Character->Jump();
	}

	AscentGravityCurve = Character->Data->AscentGravityCurve;
	DescentGravityCurve = Character->Data->DescentGravityCurve;
	JumpStartZPos = Character->GetActorLocation().Z;
	Character->MotionState = ECharacterState::Jumping;
}

void UJump::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);

	const float DistanceFromJumpStart = FMath::Abs(Character->GetActorLocation().Z - JumpStartZPos);
	UpdateGravity(DistanceFromJumpStart * 0.01 /*convert from cm/s to m/s */);

	// Check if apex is reached, if so change state to fall loop or land depending on ground distance
	if (Character->JumpState != EJumpState::JumpStart)
	{
		if (Character->GetGroundDistance() < 180)
		{
			Character->JumpState = EJumpState::JumpEnd;
		}
		else
		{
			Character->JumpState = EJumpState::JumpLoop;
		}
	}

	// Check if character is falling, if so change state to idle/walk
	if (!Character->GetCharacterMovement()->IsFalling())
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
}

void UJump::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->StopJumping();
	Character->JumpState = EJumpState::None;
}

void UJump::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
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

	//TO DO: add shoot and aim
}

void UJump::UpdateGravity(const float GroundDistance) const
{
	if (Character->GetVelocity().Z >= 0)
	{
		Character->GetCharacterMovement()->GravityScale = AscentGravityCurve->GetFloatValue(GroundDistance);
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = DescentGravityCurve->GetFloatValue(GroundDistance);
	}
}

void UJump::OnJumpApexReached()
{
	if (Character->MotionState == ECharacterState::Jumping)
	{
		Character->JumpState = EJumpState::JumpLoop;
	}
	else if (Character->MotionState != ECharacterState::Dodging && Character->MotionState !=
		ECharacterState::Hooking)
	{
		Character->ChangeMotionState(3); // Jump
	}
}
