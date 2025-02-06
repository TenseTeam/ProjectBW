// Copyright Villains, Inc. All Rights Reserved.


#include "Input/ActionManagementSystem/PlayerAction.h"

#include "Utility/FGvDebug.h"

UPlayerAction::UPlayerAction() : InputAction(nullptr), bTriggerModePressed(true), bWantPerform(false),
                                 bIsPerforming(false), bCanPerform(true)
{
}

bool UPlayerAction::Initialize(const FGameplayTag& Tag)
{
	if (!IsValid(InputAction))
	{
		FGvDebug::Warning("Invalid InputAction in PlayerAction: " + GetName() + " Action will not work");;
		return false;
	}

	ActionTag = Tag;
	if (!ActionTag.IsValid() || ActionTag == FGameplayTag::EmptyTag)
	{
		FGvDebug::Warning("Invalid ActionTag in PlayerAction: " + GetName() + " Action will not work");
		return false;
	}
	
	InputAction->Triggers.Empty();
	if (bTriggerModePressed)
	{
		UInputTriggerPressed* TriggerPressed = NewObject<UInputTriggerPressed>();
		InputAction->Triggers.Add(TriggerPressed);
	}
	else
	{
		UInputTriggerDown* TriggerDown = NewObject<UInputTriggerDown>();
		InputAction->Triggers.Add(TriggerDown);
	}
	
	UInputTriggerReleased* TriggerReleased = NewObject<UInputTriggerReleased>();
	InputAction->Triggers.Add(TriggerReleased);
	return true;
}

void UPlayerAction::OnInputActionTriggered(const FInputActionValue& Value)
{
	bWantPerform = true;
	if (!CanPerform()) return;
	OnActionTriggered.Broadcast(Value);
}

void UPlayerAction::OnInputActionCompleted(const FInputActionValue& Value) 
{
	bWantPerform = false;
	OnActionCompleted.Broadcast(Value);
}
