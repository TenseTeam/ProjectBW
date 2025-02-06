// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ActionState.h"
#include "ActionIdle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UActionIdle : public UActionState
{
	GENERATED_BODY()

	virtual void Enter(AActor* Context) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	UFUNCTION()
	void CheckWantShootOrAim();
};
