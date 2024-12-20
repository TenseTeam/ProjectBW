// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ActionState.h"
#include "Shoot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UShoot : public UActionState
{
	GENERATED_BODY()

	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	UFUNCTION()
    void InterruptShoot();
	
	bool bIsAiming = false;
	bool bIsShooting = false;
	// bool bPaused = false;
	//
	// void PauseState();
	// void UnpauseState();
};
