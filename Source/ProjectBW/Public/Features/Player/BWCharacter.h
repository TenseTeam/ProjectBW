// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/EnvironmentTracing/GroundCheckComponent.h"
#include "Data/CharacterData.h"
#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"
#include "Patterns/State/StateMachineComponent.h"
#include "BWCharacter.generated.h"

class UCharacterState;
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

UENUM(BlueprintType) 
enum class EJumpState : uint8
{
	None UMETA(DisplayName = "None"),
	JumpStart UMETA(DisplayName = "JumpStart"),
	JumpLoop UMETA(DisplayName = "JumpLoop"),
	JumpEnd UMETA(DisplayName = "JumpEnd")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementModeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyApex);

UCLASS()
class PROJECTBW_API ABWCharacter : public ABWCharacterBase
{
	GENERATED_BODY()

public:
	static FMovementModeChanged OnMovementModeChangedEvent;
	static FNotifyApex OnNotifyApexEvent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterData* Data;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState = ECharacterState::Standing;

	UPROPERTY(BlueprintReadOnly)
	EJumpState JumpState = EJumpState::None;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* StateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY()
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGroundCheckComponent* GroundCheckComponent;
	UPROPERTY()
	AGameplayController* BWController;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsShooting;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanMove;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanLook;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanRun;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanShoot;
	

public:
	ABWCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	void HandleInput(const EInputActionType InputAction, const FInputActionValue& Value) const;
	void ChangeState(const int Index) const;
	const UCharacterState* GetState(const int Index) const;

	void Move(const FVector& MoveVector);

	UFUNCTION(BlueprintCallable)
	float GetGroundDistance() const;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;
	UFUNCTION(BlueprintCallable)
	void SetIsRunning(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsShooting() const;
	UFUNCTION(BlueprintCallable)
	void SetIsShooting(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsAiming() const;
	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool Value);
	
	UFUNCTION(BlueprintCallable)
	bool CanMove() const;
	UFUNCTION(BlueprintCallable)
	bool CanLook() const;
	UFUNCTION(BlueprintCallable)
	bool CanRun() const;

private:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void NotifyJumpApex() override;
};
