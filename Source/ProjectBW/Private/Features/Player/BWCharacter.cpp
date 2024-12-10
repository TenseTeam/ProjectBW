// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/BWCharacter.h"


ABWCharacter::ABWCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<UGvSpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("StateMachineComponent");

	bCanMove = true;
	bCanLook = true;
}

void ABWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABWCharacter::HandleInput(const EInputActionType InputAction, const FInputActionValue& Value)
{
	StateMachineComponent->HandleInput(InputAction, Value);
}

void ABWCharacter::ChangeState(const int Index) const
{
	StateMachineComponent->ChangeState(Index);
}

bool ABWCharacter::CanMove()
{
	return bCanMove;
}

bool ABWCharacter::CanLook()
{
	return bCanLook;
}

const UCharacterStatsBase* ABWCharacter::GetState(const int Index) const
{
	return Cast<UCharacterStatsBase>(StateMachineComponent->GetState(Index));
}



