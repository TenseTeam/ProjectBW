// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MotionState.h"
#include "MotionIdle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UMotionIdle : public UMotionState
{
	GENERATED_BODY()

	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float deltaTime) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;
};
