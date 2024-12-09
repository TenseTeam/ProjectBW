// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EInputActionType.h"
#include "InputActionValue.h"
#include "UObject/Object.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GVUEDK_API UStateBase : public UObject
{
	GENERATED_BODY()
	
protected:
	friend class UStateMachineComponent;
	
	virtual void Initialize(AActor* Context);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(AActor* Context);
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdate(AActor* Context, float DeltaTime);
	UFUNCTION(BlueprintNativeEvent)
	void OnExit(AActor* Context);
	UFUNCTION(BlueprintNativeEvent)
	void OnHandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value);
	
	virtual void Enter(AActor* Context);
	virtual void Update(AActor* Context, float DeltaTime);
	virtual void Exit(AActor* Context);
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value);
};
