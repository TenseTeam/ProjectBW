// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterState.h"
#include "Run.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API URun : public UCharacterState
{
	GENERATED_BODY()

	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	float DefaultAcceleration;
	float DefaultSpeed;
};
