// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "PlayerAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionTriggeredDispathcer, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionCompletedDispathcer, const FInputActionValue&, Value);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class GVUEDK_API UPlayerAction : public UObject
{
	GENERATED_BODY()

public:
	UPlayerAction();

	//Returns true if the action is initialized successfully
	bool Initialize(const FGameplayTag& Tag);
	void OnInputActionTriggered(const FInputActionValue& Value);
	void OnInputActionCompleted(const FInputActionValue& Value);

	UInputAction* GetInputAction() const { return InputAction; }
	
	void SetIsPerforming(const bool Value) { bIsPerforming = Value; }
	void SetCanPerform(const bool Value) { bCanPerform = Value; }
	
	bool TriggerModeIsPressed() const { return bTriggerModePressed; }
	bool WantPerform() const { return bWantPerform; }
	bool IsPerforming() const { return bIsPerforming; }
	bool CanPerform() const { return bCanPerform; }

public:
	UPROPERTY(BlueprintAssignable)
	FActionTriggeredDispathcer OnActionTriggered;
	UPROPERTY(BlueprintAssignable)
	FActionCompletedDispathcer OnActionCompleted;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bTriggerModePressed;

	FGameplayTag ActionTag;
	
	bool bWantPerform;
	bool bIsPerforming;
	bool bCanPerform;
};
