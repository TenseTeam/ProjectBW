// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ActionState.h"
#include "Reload.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UReload : public UActionState
{
	GENERATED_BODY()

	virtual void Initialize(AActor* Context) override;
	virtual void Enter(AActor* Context) override;
	virtual void Update(AActor* Context, float DeltaTime) override;
	virtual void Exit(AActor* Context) override;

	UFUNCTION()
	void InterruptReload();
	void ReloadCompleted();

	FTimerHandle ReloadTimer;

	float ReloadDuration;
};

