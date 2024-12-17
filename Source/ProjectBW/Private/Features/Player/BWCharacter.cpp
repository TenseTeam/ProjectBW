// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/BWCharacter.h"

#include <string>

#include "Features/Player/States/Base/CharacterState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UGameFramework/Controllers/GameplayController.h"
#include "Utility/FGvDebug.h"

FMovementModeChanged ABWCharacter::OnMovementModeChangedEvent;
FNotifyApex ABWCharacter::OnNotifyApexEvent;

ABWCharacter::ABWCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;

	SpringArm = CreateDefaultSubobject<UGvSpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("StateMachineComponent");

	GroundCheckComponent = CreateDefaultSubobject<UGroundCheckComponent>("GroundCheckComponent");
	
	GrapplingHook = CreateDefaultSubobject<UGrapplingHookComponent>("Grappling Hook");

	bCanMove = true;
	bCanLook = true;
	bCanRun = true;
	bCanDodge = true;
	bCanHook = true;

}

void ABWCharacter::BeginPlay()
{
	Super::BeginPlay();
	BWController = Cast<AGameplayController>(GetController());
	if (!BWController)
	{
		FGvDebug::Warning(GetName() + " Has Invalid Controller, input will not work", true);
	}
}

void ABWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//State Machine Stuff...

void ABWCharacter::HandleInput(const EInputActionType InputAction, const FInputActionValue& Value) const
{
	StateMachineComponent->HandleInput(InputAction, Value);
}

void ABWCharacter::ChangeState(const int Index) const
{
	StateMachineComponent->ChangeState(Index);
}

const UCharacterState* ABWCharacter::GetState(const int Index) const
{
	return Cast<UCharacterState>(StateMachineComponent->GetState(Index));
}

//End State Machine Stuff


//Character Movement Stuff...

void ABWCharacter::Move(const FVector& MoveVector)
{
	FRotator CameraCurrentRotation = BWController->PlayerCameraManager->GetCameraCacheView().Rotation;
	FRotator MoveVectorRotationOffset(0, CameraCurrentRotation.Yaw, 0);
	AddMovementInput(MoveVectorRotationOffset.Quaternion() * MoveVector);
}

UGrapplingHookComponent* ABWCharacter::GetGrapplingHook() const
{
	return GrapplingHook;
}

UGvSpringArmComponent* ABWCharacter::GetSpringArm() const
{
    return SpringArm;
}

float ABWCharacter::GetGroundDistance() const
{
	const float Distance = GroundCheckComponent->CheckGroundDistance();
	return Distance == -1 ? GroundCheckComponent->GetMaxGroundDistance() : Distance;
}

bool ABWCharacter::MoveInputActive() const
{
	return !AGameplayController::GetMoveInputValue().IsNearlyZero();
}

bool ABWCharacter::IsRunning() const
{
	return bIsRunning && !AGameplayController::GetMoveInputValue().IsNearlyZero();
}

void ABWCharacter::SetIsRunning(bool Value)
{
	bIsRunning = Value;
}

bool ABWCharacter::IsShooting() const
{
	return bIsShooting;
}

void ABWCharacter::SetIsShooting(bool Value)
{
	bIsShooting = Value;
}

bool ABWCharacter::IsAiming() const
{
	return bIsAiming;
}

void ABWCharacter::SetIsAiming(bool Value)
{
	bIsAiming = Value;
}

bool ABWCharacter::IsHooking() const
{
	return bIsHooking;
}

void ABWCharacter::SetIsHooking(bool Value)
{
	bIsHooking = Value;
}

bool ABWCharacter::CanMove() const
{
	return bCanMove;
}

bool ABWCharacter::CanLook() const
{
	return bCanLook;
}

bool ABWCharacter::CanRun() const
{
	return bCanRun;
}

void ABWCharacter::SetCanDodge(bool Value)
{
	bCanDodge = Value;
}

bool ABWCharacter::CanDodge() const
{
	return bCanDodge;
}

void ABWCharacter::SetCanHook(bool Value)
{ 
	bCanHook = Value;
}

bool ABWCharacter::CanHook() const
{
	return bCanHook && GrapplingHook->IsTargetAcquired();
}

void ABWCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	GetCharacterMovement()->bNotifyApex = GetCharacterMovement()->MovementMode == MOVE_Falling;

	if (OnMovementModeChangedEvent.IsBound())
	{
		OnMovementModeChangedEvent.Broadcast();
	}
}

void ABWCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	if (OnNotifyApexEvent.IsBound())
	{
		OnNotifyApexEvent.Broadcast();
	}
}
