// Copyright Villains, Inc. All Rights Reserved.


#include "UGameFramework/Controllers/GameplayController.h"
#include "EnhancedInputComponent.h"
#include "Features/Gameplay/InventorySystem/BWInventory.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/CharacterStats/CharacterStats.h"
#include "Utility/FGvDebug.h"

FVector AGameplayController::MoveInputValue = FVector::ZeroVector;

AGameplayController::AGameplayController()
{
	Inventory = CreateDefaultSubobject<UBWInventory>("Inventory");
	Equipment = CreateDefaultSubobject<UEquipment>("Equipment");
	CharacterStats = CreateDefaultSubobject<UCharacterStats>("CharacterStats");
}

void AGameplayController::BeginPlay()
{

	Inventory->LinkEquipment(Equipment);
	Super::BeginPlay();
	CharacterStats->Init(Equipment);
	
	BWCharacter = Cast<ABWCharacter>(GetCharacter());
	if (!BWCharacter)
	{
		FGvDebug::Warning(GetName() + " Has Invalid Character, input will not work", true);
		return;
	}
	
	ContextsManager->SetGameplayMappingContext();
}

void AGameplayController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameplayController::Walk);
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGameplayController::StopWalk);
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameplayController::Look);
		enhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AGameplayController::Run);
		enhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AGameplayController::StopRun);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGameplayController::Jump);
		enhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AGameplayController::Dodge);
		enhancedInputComponent->BindAction(ThrowHookAction, ETriggerEvent::Triggered, this, &AGameplayController::ThrowHook);
		enhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGameplayController::Shoot);
		enhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AGameplayController::StopShoot);
		enhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AGameplayController::Aim);
		enhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AGameplayController::StopAim);
		enhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AGameplayController::Interact);
		enhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AGameplayController::Reload);
		EnhancedInputComponent = enhancedInputComponent;
	}
}

//Controller input functions

void AGameplayController::Look(const FInputActionValue& Value)
{
	if (!BWCharacter->CanLook()) return;
	const FVector LookVector = Value.Get<FVector>();
	AddYawInput(LookVector.X);
	AddPitchInput(-LookVector.Y);
}

void AGameplayController::Walk(const FInputActionValue& Value)
{
	MoveInputValue = Value.Get<FVector>();
	if (!BWCharacter->CanMove()) return;
	BWCharacter->HandleMotionInput(EInputActionType::Walk, Value);
}

void AGameplayController::StopWalk(const FInputActionValue& Value)
{
	MoveInputValue = FVector::ZeroVector;
}

void AGameplayController::Run(const FInputActionValue& Value)
{
	BWCharacter->SetWantRunning(true);
	if (!BWCharacter->CanRun()) return;
	BWCharacter->HandleMotionInput(EInputActionType::Run, Value);
}

void AGameplayController::StopRun(const FInputActionValue& Value)
{
	BWCharacter->SetWantRunning(false);
}

void AGameplayController::Jump(const FInputActionValue& Value)
{
	//if (BWCharacter->JumpState != EJumpState::None) return;
	BWCharacter->HandleMotionInput(EInputActionType::Jump, Value);
}

void AGameplayController::Dodge(const FInputActionValue& Value)
{
	if (!BWCharacter->CanDodge()) return;
	BWCharacter->HandleMotionInput(EInputActionType::Dodge, Value);
}

void AGameplayController::ThrowHook(const FInputActionValue& Value)
{
	if (!BWCharacter->CanHook()) return;
	BWCharacter->HandleMotionInput(EInputActionType::Hook, Value);
}

void AGameplayController::Shoot(const FInputActionValue& Value)
{
	BWCharacter->SetWantShooting(true);
	if (!BWCharacter->CanShoot()) return;
	BWCharacter->HandleActionInput(EInputActionType::Shoot, Value);
}

void AGameplayController::StopShoot(const FInputActionValue& Value)
{
	BWCharacter->SetWantShooting(false);
}

void AGameplayController::Aim(const FInputActionValue& Value)
{
	BWCharacter->SetWantAiming(true);
	if (!BWCharacter->CanShoot()) return;
	BWCharacter->HandleActionInput(EInputActionType::Aim, Value);
}

void AGameplayController::StopAim(const FInputActionValue& Value)
{
	BWCharacter->SetWantAiming(false);
}

void AGameplayController::Interact(const FInputActionValue& Value)
{
	if (!BWCharacter->CanInteract()) return;
	BWCharacter->HandleActionInput(EInputActionType::Interact, Value);
}

void AGameplayController::Reload(const FInputActionValue& Value)
{
	if (!BWCharacter->CanReload()) return;
	BWCharacter->HandleActionInput(EInputActionType::Reload, Value);
}

