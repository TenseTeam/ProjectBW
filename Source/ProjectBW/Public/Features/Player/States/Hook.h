// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MotionState.h"
#include "Hook.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UHook : public UMotionState
{
	GENERATED_BODY()

	UPROPERTY()
	UGrapplingHookComponent* PlayerGrapplingHook;
	float HookEndNormalizedDistance;
	float StartDelay;
	float ElapsedTime;
	
	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float deltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	UFUNCTION()
	void OnHookFinished();
	
};
