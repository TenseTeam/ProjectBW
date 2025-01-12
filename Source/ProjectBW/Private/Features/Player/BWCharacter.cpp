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

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(SpringArm);

	MotionStateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("MotionStateMachineComponent");
	ActionStateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("ActionStateMachineComponent");

	GroundCheckComponent = CreateDefaultSubobject<UGroundCheckComponent>("GroundCheckComponent");
	
	GrapplingHook = CreateDefaultSubobject<UGrapplingHookComponent>("Grappling Hook");

	DodgerComponent = CreateDefaultSubobject<UDodgerComponent>("DodgerComponent");

	bCanMove = true;
	bCanLook = true;
	bCanRun = true;
	bCanDodge = true;
	bCanHook = true;
	bCanShoot = true;

}

void ABWCharacter::BeginPlay()
{
	Super::BeginPlay();
	BWController = Cast<AGameplayController>(GetController());
	if (!BWController)
	{
		FGvDebug::Warning(GetName() + " Has Invalid Controller, input will not work", true);
		return;
	}

	GrapplingHook->OnStartHooking.AddDynamic(this, &ABWCharacter::StartHooking);
	GrapplingHook->OnStopHooking.AddDynamic(this, &ABWCharacter::StopHooking);

	DodgerComponent->OnStartDodge.AddDynamic(this, &ABWCharacter::StartDodging);
	DodgerComponent->OnDodge.AddDynamic(this, &ABWCharacter::Dodging);
	DodgerComponent->OnStopDodge.AddDynamic(this, &ABWCharacter::StopDodging);

	InitStats();
}

void ABWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GrapplingHook->OnStartHooking.RemoveDynamic(this, &ABWCharacter::StartHooking);
	GrapplingHook->OnStopHooking.RemoveDynamic(this, &ABWCharacter::StopHooking);
}

void ABWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABWCharacter::HandleMotionInput(const EInputActionType InputAction, const FInputActionValue& Value) const
{
	MotionStateMachineComponent->HandleInput(InputAction, Value);
}

void ABWCharacter::ChangeMotionState(const int Index) const
{
	MotionStateMachineComponent->ChangeState(Index);
}

const UCharacterState* ABWCharacter::GetMotionState(const int Index) const
{
	return Cast<UCharacterState>(MotionStateMachineComponent->GetState(Index));
}

const UCharacterState* ABWCharacter::GetCurrentMotionState() const
{
	return Cast<UCharacterState>(MotionStateMachineComponent->GetCurrentState());
}

const UCharacterState* ABWCharacter::GetPreviousMotionState() const
{
	return Cast<UCharacterState>(MotionStateMachineComponent->GetPreviousState());
}

void ABWCharacter::HandleActionInput(const EInputActionType InputAction, const FInputActionValue& Value) const
{
	ActionStateMachineComponent->HandleInput(InputAction, Value);
}

void ABWCharacter::ChangeActionState(const int Index) const
{
	ActionStateMachineComponent->ChangeState(Index);
}

const UCharacterState* ABWCharacter::GetActionState(const int Index) const
{
	return Cast<UCharacterState>(ActionStateMachineComponent->GetState(Index));
}

void ABWCharacter::Move(const FVector& MoveVector)
{
	const FRotator CameraCurrentRotation = BWController->PlayerCameraManager->GetCameraCacheView().Rotation;
	const FRotator MoveVectorRotationOffset(0, CameraCurrentRotation.Yaw, 0);
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

UDodgerComponent* ABWCharacter::GetDodgerComponent() const
{
	return DodgerComponent;
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
	return bIsRunning && bWantRunning && MoveInputActive();
}

void ABWCharacter::SetIsRunning(bool Value)
{
	bIsRunning = Value;
}

bool ABWCharacter::WantRunning() const
{
	return bWantRunning;
}

void ABWCharacter::SetWantRunning(bool Value)
{
	bWantRunning = Value;
}


bool ABWCharacter::IsShooting() const
{
	return bIsShooting;
}

void ABWCharacter::SetIsShooting(bool Value)
{
	bIsShooting = Value;
}

bool ABWCharacter::WantShooting() const
{
	return bWantShooting;
}

void ABWCharacter::SetWantShooting(bool Value)
{
	bWantShooting = Value;
}

bool ABWCharacter::IsAiming() const
{
	return bIsAiming;
}

void ABWCharacter::SetIsAiming(bool Value)
{
	bIsAiming = Value;
}

bool ABWCharacter::WantAiming() const
{
	return bWantAiming;
}

void ABWCharacter::SetWantAiming(bool Value)
{
	bWantAiming = Value;
}

bool ABWCharacter::IsHooking() const
{
	return bIsHooking;
}

void ABWCharacter::SetIsHooking(bool Value)
{
	bIsHooking = Value;
}

bool ABWCharacter::IsDodging() const
{
	return bIsDodging;
}

void ABWCharacter::SetIsDodging(bool Value)
{
	bIsDodging = Value;
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
	return bCanDodge && !DodgerComponent->IsCoolingDown();
}

void ABWCharacter::SetCanHook(bool Value)
{ 
	bCanHook = Value;
}

bool ABWCharacter::CanHook() const
{
	return bCanHook && GrapplingHook->IsTargetAcquired();
}

void ABWCharacter::SetCanShoot(bool Value)
{
	bCanShoot = Value;
}

bool ABWCharacter::CanShoot() const
{
	return bCanShoot && !IsDodging() && !IsHooking();
}

void ABWCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	//Needed to notify the apex of the jump/fall
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

void ABWCharacter::StartHooking()
{
	SetIsHooking(true); // perdoname madre por mi vida loca
	OnStartHook.Broadcast();
}

void ABWCharacter::StopHooking()
{
	SetIsHooking(false); // perdoname madre por mi vida loca
	OnStopHook.Broadcast();
}

void ABWCharacter::StartDodging()
{
	OnStartDodging.Broadcast();
}

void ABWCharacter::Dodging()
{
	OnDodging.Broadcast();
}

void ABWCharacter::StopDodging()
{
	OnStopDodging.Broadcast();
}

void ABWCharacter::InitStats()
{
	UpdateStats();
}

void ABWCharacter::UpdateStats()
{
	GetCharacterMovement()->MaxWalkSpeed = Data->WalkSpeed;
}
