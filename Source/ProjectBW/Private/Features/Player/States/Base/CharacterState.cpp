// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Base/CharacterState.h"

#include "Utility/FGvDebug.h"

void UCharacterState::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	Character = Cast<ABWCharacter>(Context);
	
	if (!Character)
	{
		FGvDebug::Error(GetName() + " Has Invalid Context, Initialization Failed");
		bInitialized = false;
		return;
	}

	Controller = Cast<AGameplayController>(Character->GetController());
	if (!Controller)
	{
		FGvDebug::Error(GetName() + " Has Invalid Controller, Initialization Failed");
		bInitialized = false;
		return;
	}
	
	bInitialized = true;
}

void UCharacterState::Enter(AActor* Context)
{
	Super::Enter(Context);
	FGvDebug::Log(GetName() + " State Entered");
}

void UCharacterState::Update(AActor* Context, float DeltaTime)
{
	Super::Update(Context, DeltaTime);
}

void UCharacterState::Exit(AActor* Context)
{
	Super::Exit(Context);
}

void UCharacterState::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	Super::HandleInput(Context, InputAction, Value);
}
