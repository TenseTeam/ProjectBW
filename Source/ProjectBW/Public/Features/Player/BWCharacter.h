// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/Dodger/DodgerComponent.h"
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
	None UMETA(DisplayName = "None"),
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

//Motion events (Unreal Engine Events)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementModeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyApex);

//Shooting events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartShooting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShooting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopShooting);

//Aiming events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopAiming);

//Running events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartRunning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopRunning);

//Dodge events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDodging);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDodging);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopDodging);

//Dodge events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartHook);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopHook);


UCLASS()
class PROJECTBW_API ABWCharacter : public ABWCharacterBase
{
	GENERATED_BODY()

public:
	static FMovementModeChanged OnMovementModeChangedEvent;
	static FNotifyApex OnNotifyApexEvent;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartShooting OnStartShooting;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FShooting OnShooting;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopShooting OnStopShooting;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartAiming OnStartAiming;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FAiming OnAiming;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopAiming OnStopAiming;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartRunning OnStartRunning;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopRunning OnStopRunning;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartDodging OnStartDodging;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FDodging OnDodging;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopDodging OnStopDodging;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartHook OnStartHook;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopHook OnStopHook;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterData* Data;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState MotionState = ECharacterState::Standing;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState ActionState = ECharacterState::None;

	UPROPERTY(BlueprintReadOnly)
	EJumpState JumpState = EJumpState::None;

	UPROPERTY(BlueprintReadOnly)
	EHookState HookState = EHookState::None;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* MotionStateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* ActionStateMachineComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UGroundCheckComponent* GroundCheckComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrapplingHookComponent* GrapplingHook;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDodgerComponent* DodgerComponent;
	
	UPROPERTY()
	AGameplayController* BWController;

	UPROPERTY()
	bool bIsRunning;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bWantRunning;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bWantShooting;
	UPROPERTY()
	bool bIsShooting;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bWantAiming;
	UPROPERTY()
	bool bIsAiming;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHooking;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsDodging;
	
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void HandleMotionInput(const EInputActionType InputAction, const FInputActionValue& Value) const;
	void ChangeMotionState(const int Index) const;
	const UCharacterState* GetMotionState(const int Index) const;
	const UCharacterState* GetCurrentMotionState() const;
	const UCharacterState* GetPreviousMotionState() const;

	void HandleActionInput(const EInputActionType InputAction, const FInputActionValue& Value) const;
	void ChangeActionState(const int Index) const;
	const UCharacterState* GetActionState(const int Index) const;

	void Move(const FVector& MoveVector);

	UFUNCTION(BlueprintCallable)
	UGrapplingHookComponent* GetGrapplingHook() const;
	UFUNCTION(BlueprintCallable)
	UGvSpringArmComponent* GetSpringArm() const;
	UFUNCTION(BlueprintCallable)
	UDodgerComponent* GetDodgerComponent() const;

	UFUNCTION(BlueprintCallable)
	float GetGroundDistance() const;
	UFUNCTION(BlueprintCallable)
	bool MoveInputActive() const;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const;
	UFUNCTION(BlueprintCallable)
	void SetIsRunning(bool Value);
	UFUNCTION(BlueprintCallable)
	bool WantRunning() const;
	UFUNCTION(BlueprintCallable)
	void SetWantRunning(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsShooting() const;
	UFUNCTION(BlueprintCallable)
	void SetIsShooting(bool Value);
	UFUNCTION(BlueprintCallable)
	bool WantShooting() const;
	UFUNCTION(BlueprintCallable)
	void SetWantShooting(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsAiming() const;
	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool Value);
	UFUNCTION(BlueprintCallable)
	bool WantAiming() const;
	UFUNCTION(BlueprintCallable)
	void SetWantAiming(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsHooking() const;
	UFUNCTION(BlueprintCallable)
	void SetIsHooking(bool Value);

	UFUNCTION(BlueprintCallable)
	bool IsDodging() const;
	UFUNCTION(BlueprintCallable)
	void SetIsDodging(bool Value);
	
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

	UFUNCTION(BlueprintCallable)
	void SetCanShoot(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;

private:
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void NotifyJumpApex() override;
	
	UFUNCTION()
	void StartHooking();
	UFUNCTION()
	void StopHooking();

	UFUNCTION()
	void StartDodging();
	UFUNCTION()
	void Dodging();
	UFUNCTION()
	void StopDodging();

	void InitStats();
	void UpdateStats();
};
