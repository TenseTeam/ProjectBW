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

class AGameplayController;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Standing UMETA(DisplayName = "Standing"),
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	Dodging UMETA(DisplayName = "Dodging"),
	Shooting UMETA(DisplayName = "Shooting"),
	Hooking UMETA(DisplayName = "Hooking"),
	Climbing UMETA(DisplayName = "Climbing")
};

UCLASS()
class PROJECTBW_API ABWCharacter : public ABWCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterData* Data;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;

	

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* StateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY()
	UCameraComponent* Camera;
	UPROPERTY()
	AGameplayController* BWController;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanMove;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanLook;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanRun;
	

public:
	ABWCharacter();
	virtual void Tick(float DeltaTime) override;
	
	void HandleInput(const EInputActionType InputAction, const FInputActionValue& Value);
	void ChangeState(const int Index) const;
	const UCharacterStatsBase* GetState(const int Index) const;

	void Move(const FVector& MoveVector);

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;
	UFUNCTION(BlueprintCallable)
	void SetIsRunning(bool Value);
	
	UFUNCTION(BlueprintCallable)
	bool CanMove() const;
	UFUNCTION(BlueprintCallable)
	bool CanLook() const;
	UFUNCTION(BlueprintCallable)
	bool CanRun() const;

protected:
	virtual void BeginPlay() override;
};
