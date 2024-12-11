// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterState.h"
#include "Jump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UJump : public UCharacterState
{
	GENERATED_BODY()

	UPROPERTY()
	UCurveFloat* AscentGravityCurve;
	UPROPERTY()
	UCurveFloat* DescentGravityCurve;

	float JumpStartZPos;

	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	void UpdateGravity(const float GroundDistance) const;
	UFUNCTION()
	void OnJumpApexReached();
	
};


