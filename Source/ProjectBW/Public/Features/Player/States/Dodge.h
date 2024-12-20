// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MotionState.h"
#include "Dodge.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UDodge : public UMotionState
{
	GENERATED_BODY()

	float DodgeSpeed = 0.f;
	float DodgeCooldown = 0.f;
	float RotationSpeed = 0.f;
	float DefaultBrakingDecelerationWalking = 0.f;
	
	FVector TargetVelocity;
	FRotator TargetRotation;
	FRotator CurrentRotation;

	UPROPERTY()
	UAnimMontage* DodgeAnimMontage;
	float DodgeAnimPlayRate = 0.f;
	float DodgeAnimLenght;
	float ExitNormalizedTime;

	float ElapsedTime;

	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;
	
	FVector GetRollVelocity() const;
	void StartCooldown() const;
};
