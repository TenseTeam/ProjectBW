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

	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;

	UPROPERTY()
	UDodgerComponent* PlayerDodgeComponent;
	
	UFUNCTION()
	void OnDodgeFinished();
};
