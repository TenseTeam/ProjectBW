// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/EnvironmentTracing/GroundCheckComponent.h"
#include "Data/CharacterData.h"
#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"
#include "Features/Gameplay/GrapplingHookSystem/GrapplingHookComponent.h"
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
	None = 0 UMETA(DisplayName = "None"),
	JumpStart = 1 UMETA(DisplayName = "JumpStart"),
	JumpLoop = 2 UMETA(DisplayName = "JumpLoop"),
	JumpEnd = 3 UMETA(DisplayName = "JumpEnd")
};

UENUM(BlueprintType) 
enum class EHookState : uint8
{
	None UMETA(DisplayName = "None"),
	HookStart UMETA(DisplayName = "HookStart"),
	HookLoop UMETA(DisplayName = "HookLoop"),
	HookEnd UMETA(DisplayName = "HookEnd")
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

	UPROPERTY(BlueprintReadOnly)
	EHookState HookState = EHookState::None;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* StateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGroundCheckComponent* GroundCheckComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrapplingHookComponent* GrapplingHook;
	UPROPERTY()
	UCameraComponent* Camera;
	UPROPERTY()
	AGameplayController* BWController;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsShooting;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHooking;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanMove;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanLook;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanRun;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanDodge;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanShoot;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanHook;
	

public:
	ABWCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	void HandleInput(const EInputActionType InputAction, const FInputActionValue& Value) const;
	void ChangeState(const int Index) const;
	const UCharacterState* GetState(const int Index) const;

	void Move(const FVector& MoveVector);

	UFUNCTION(BlueprintCallable)
	UGrapplingHookComponent* GetGrapplingHook() const;
	UFUNCTION(BlueprintCallable)
	UGvSpringArmComponent* GetSpringArm() const;

	UFUNCTION(BlueprintCallable)
	float GetGroundDistance() const;
	UFUNCTION(BlueprintCallable)
	bool MoveInputActive() const;

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
	bool IsHooking() const;
	UFUNCTION(BlueprintCallable)
	void SetIsHooking(bool Value);
	
	UFUNCTION(BlueprintCallable)
	bool CanMove() const;
	UFUNCTION(BlueprintCallable)
	bool CanLook() const;
	UFUNCTION(BlueprintCallable)
	bool CanRun() const;

	UFUNCTION(BlueprintCallable)
	void SetCanDodge(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CanDodge() const;

	UFUNCTION(BlueprintCallable)
	void SetCanHook(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CanHook() const;

private:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void NotifyJumpApex() override;
};
