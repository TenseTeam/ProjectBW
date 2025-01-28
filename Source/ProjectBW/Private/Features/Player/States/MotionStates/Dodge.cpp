// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/MotionStates/Dodge.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void UDodge::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	PlayerDodgeComponent = Character->GetDodgerComponent();
}

void UDodge::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);


	FRotator OffsetRotation = Character->GetControlRotation();
	OffsetRotation.Pitch = 0;
	OffsetRotation.Roll = 0;
	PlayerDodgeComponent->StartDodge(OffsetRotation.Quaternion() * Controller->GetMoveInputValue());

	Character->MotionState = ECharacterState::Dodging;
	Character->SetIsDodging(true);
	
	PlayerDodgeComponent->OnStopDodge.AddDynamic(this, &UDodge::OnDodgeFinished);
}

void UDodge::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);
}

void UDodge::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->SetIsDodging(false);
	PlayerDodgeComponent->OnStopDodge.RemoveDynamic(this, &UDodge::OnDodgeFinished);
}

void UDodge::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);
}

void UDodge::OnDodgeFinished()
{
	if (Character->GetCharacterMovement()->IsFalling())
	{
		Character->ChangeMotionState(3); //jump
		return;
	}
						
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		Character->ChangeMotionState(0); //idle
		return;
	}
													
	Character->ChangeMotionState(1); //walk
}
