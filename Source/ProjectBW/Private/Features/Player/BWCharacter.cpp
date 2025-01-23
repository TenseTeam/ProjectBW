// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/BWCharacter.h"

#include <string>

#include "Features/Gameplay/InteractionSystem/Components/InteractableDetectorComponent.h"
#include "Features/Gameplay/ResourceAttributeSystem/HealthAttribute.h"
#include "Features/Gameplay/ResourceAttributeSystem/ShieldAttribute.h"
#include "Features/Gameplay/ResourceAttributeSystem/StaminaAttribute.h"
#include "Features/Gameplay/ResourceAttributeSystem/Base/ResourceAttributeBase.h"
#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/WeaponsSwitcher.h"
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

	InteractableDetector = CreateDefaultSubobject<UInteractableDetectorComponent>("InteractableDetector");

	AttributeManager = CreateDefaultSubobject<UResourceAttributeManager>("AttributeManager");

	WeaponsSwitcher = CreateDefaultSubobject<UWeaponsSwitcher>("WeaponsSwitcher");
	
	bCanMove = true;
	bCanLook = true;
	bCanRun = true;
	bCanDodge = true;
	bCanHook = true;
	bCanShoot = true;
	bCanInteract = true;

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

	InitAttributes();
	UpdateCharacterData();
}

void ABWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GrapplingHook->OnStartHooking.RemoveDynamic(this, &ABWCharacter::StartHooking);
	GrapplingHook->OnStopHooking.RemoveDynamic(this, &ABWCharacter::StopHooking);
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

UCameraComponent* ABWCharacter::GetFollowCamera() const
{
	return FollowCamera;
}

UInteractableDetectorComponent* ABWCharacter::GetInteractableDetector() const
{
	return InteractableDetector;
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

void ABWCharacter::SetCanRun(bool Value)
{
	bCanRun = Value;
}

void ABWCharacter::SetCanDodge(bool Value)
{
	bCanDodge = Value;
}

bool ABWCharacter::CanDodge() const
{
	return bCanDodge && !DodgerComponent->IsCoolingDown() && Stamina->GetCurrentValue() >= 30.f;
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

void ABWCharacter::SetCanInteract(bool Value)
{
	bCanInteract = Value;
}

bool ABWCharacter::CanInteract() const
{
	return bCanInteract;
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

void ABWCharacter::LostHealth()
{
	OnLostHealth.Broadcast();
}

void ABWCharacter::GainedHealth()
{
	OnGainedHealth.Broadcast();
}

void ABWCharacter::Death()
{
	OnDeath.Broadcast();
}

void ABWCharacter::LostStamina()
{
	if (CanRun() && Stamina->GetCurrentValue() <= KINDA_SMALL_NUMBER)
		SetCanRun(false);
	OnLostStamina.Broadcast();
}

void ABWCharacter::GainedStamina()
{
	if (!CanRun() && Stamina->GetCurrentValue() > 30.f)
		SetCanRun(true);
	OnGainedStamina.Broadcast();
}

void ABWCharacter::StaminaEmptied()
{
	SetCanRun(false);
	OnStaminaEmptied.Broadcast();
}

void ABWCharacter::LostShield()
{
	OnLostShield.Broadcast();
}

void ABWCharacter::GainedShield()
{
	OnGainedShield.Broadcast();
}

void ABWCharacter::ShieldEmptied()
{
	OnShieldEmptied.Broadcast();
}

void ABWCharacter::UpdateCharacterData() const
{
	GetCharacterMovement()->MaxWalkSpeed = Data->WalkSpeed;
}

void ABWCharacter::InitAttributes()
{
	if (IsValid(Health))
	{
		Health->OnReachedMinValue.AddDynamic(this, &ABWCharacter::Death);
		Health->OnDecreasedAttribute.AddDynamic(this, &ABWCharacter::LostHealth);
		Health->OnIncreasedAttribute.AddDynamic(this, &ABWCharacter::GainedHealth);
		Health->SetValue(Health->MaxValue);
	}
	if (IsValid(Stamina))
	{
		Stamina->OnReachedMinValue.AddDynamic(this, &ABWCharacter::StaminaEmptied);
		Stamina->OnDecreasedAttribute.AddDynamic(this, &ABWCharacter::LostStamina);
		Stamina->OnIncreasedAttribute.AddDynamic(this, &ABWCharacter::GainedStamina);
		Stamina->SetValue(Stamina->MaxValue);
	}
	if (IsValid(Shield))
	{
		Shield->OnReachedMinValue.AddDynamic(this, &ABWCharacter::ShieldEmptied);
		Shield->OnDecreasedAttribute.AddDynamic(this, &ABWCharacter::LostShield);
		Shield->OnIncreasedAttribute.AddDynamic(this, &ABWCharacter::GainedShield);
		Shield->SetValue(Shield->MaxValue);
	}
}
