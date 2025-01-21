// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MotionState.h"
#include "Walk.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UWalk : public UMotionState
{
	GENERATED_BODY()

	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;
 
	UFUNCTION()
	void CheckWantRun();
};
