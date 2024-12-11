// Copyright Villains, Inc. All Rights Reserved.


#include "UGameFramework/Controllers/GameplayController.h"
#include "EnhancedInputComponent.h"
#include "Utility/FGvDebug.h"

FVector AGameplayController::MoveInputValue = FVector::ZeroVector;

AGameplayController::AGameplayController()
{
}

void AGameplayController::BeginPlay()
{
	Super::BeginPlay();
	BWCharacter = Cast<ABWCharacter>(GetCharacter());
	AddMappingContext();
	
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
		EnhancedInputComponent = enhancedInputComponent;
	}
}

void AGameplayController::AddMappingContext() const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
		else
		{
			FGvDebug::Error(GetName() + " Has Invalid Mapping Context, input will not work", true);
		}
	}
}

void AGameplayController::Walk(const FInputActionValue& Value)
{
	MoveInputValue = Value.Get<FVector>();
	if (!BWCharacter->CanMove()) return;
	BWCharacter->HandleInput(EInputActionType::Walk, Value);
}

void AGameplayController::StopWalk(const FInputActionValue& Value)
{
	MoveInputValue = FVector::ZeroVector;
}

void AGameplayController::Look(const FInputActionValue& Value)
{
	if (!BWCharacter->CanLook()) return;
	const FVector LookVector = Value.Get<FVector>();
	AddYawInput(LookVector.X);
	AddPitchInput(-LookVector.Y);
}

void AGameplayController::Jump(const FInputActionValue& Value)
{
	if (!BWCharacter->CanJump()) return;
	BWCharacter->HandleInput(EInputActionType::Jump, Value);
}

void AGameplayController::Run(const FInputActionValue& Value)
{
	if (!BWCharacter->CanRun()) return;
	BWCharacter->HandleInput(EInputActionType::Run, Value);
}

void AGameplayController::StopRun(const FInputActionValue& Value)
{
	BWCharacter->SetIsRunning(false);
}
