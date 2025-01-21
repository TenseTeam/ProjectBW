// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Player/BWCharacter.h"
#include "Patterns/State/StateBase.h"
#include "UGameFramework/Controllers/GameplayController.h"
#include "CharacterState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UCharacterState : public UStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ABWCharacter* Character;
	UPROPERTY()
	AGameplayController* Controller;

protected:
	bool bInitialized = false;

protected:
	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;
	virtual void HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value) override;
	
};
