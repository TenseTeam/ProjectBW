// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Data/CharacterData.h"
#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/CharacterStatsBase.h"
#include "Patterns/State/StateMachineComponent.h"
#include "BWCharacter.generated.h"

UCLASS()
class PROJECTBW_API ABWCharacter : public ABWCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterData* Data;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* StateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY()
	UCameraComponent* Camera;

	bool bCanMove;
	bool bCanLook;
	

public:
	ABWCharacter();
	virtual void Tick(float DeltaTime) override;
	
	void HandleInput(const EInputActionType InputAction, const FInputActionValue& Value);
	void ChangeState(const int Index) const;
	const UCharacterStatsBase* GetState(const int Index) const;

	UFUNCTION(BlueprintCallable)
	bool CanMove();
	UFUNCTION(BlueprintCallable)
	bool CanLook();

protected:
	virtual void BeginPlay() override;
};
