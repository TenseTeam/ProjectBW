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

class ABWWeaponFirearm;
class UWeaponsSwitcher;
class UShieldAttribute;
class UStaminaAttribute;
class UHealthAttribute;
class UResourceAttributeManager;
class UInteractableDetectorComponent;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRunning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopRunning);

//Dodge events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDodging);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDodging);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopDodging);

//Dodge events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartHook);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopHook);

//Interaction events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor*, Interactable);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGainedHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGainedStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaEmptied);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGainedShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldEmptied);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartReloading);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloading);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopReloading);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadInterrupted);


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
	FRunning OnRunning;
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

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FLostHealth OnLostHealth;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FGainedHealth OnGainedHealth;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FLostStamina OnLostStamina;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FGainedStamina OnGainedStamina;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStaminaEmptied OnStaminaEmptied;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FLostShield OnLostShield;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FGainedShield OnGainedShield;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FShieldEmptied OnShieldEmptied;

	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStartReloading OnStartReloading;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FReloading OnReloading;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FStopReloading OnStopReloading;
	UPROPERTY(BlueprintAssignable, Category = "BW Character Events")
	FReloadInterrupted OnReloadInterrupted;
	
	
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGvSpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGroundCheckComponent* GroundCheckComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrapplingHookComponent* GrapplingHook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDodgerComponent* DodgerComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInteractableDetectorComponent* InteractableDetector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UResourceAttributeManager* AttributeManager;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWeaponsSwitcher* WeaponsSwitcher;
	
	UPROPERTY()
	AGameplayController* BWController;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UHealthAttribute* Health; //initialized in blueprint event graph
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UShieldAttribute* Shield; //initialized in blueprint event graph
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaminaAttribute* Stamina; //initialized in blueprint event graph

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ABWWeaponFirearm* HoldedWeapon;

	bool bWantRunning;
    bool bWantShooting;
	bool bWantAiming;
	
	bool bIsRunning;
	bool bIsShooting;
	bool bIsAiming;
	bool bIsHooking;
	bool bIsDodging;
	bool bIsReloading;
	
	bool bCanMove;
	bool bCanLook;
	bool bCanRun;
	bool bCanDodge;
	bool bCanShoot;
	bool bCanHook;
	bool bCanInteract;
	bool bCanReload;
	

public:
	ABWCharacter();
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
	UCameraComponent* GetFollowCamera() const;
	UFUNCTION(BlueprintCallable)
	UInteractableDetectorComponent* GetInteractableDetector() const;
	UFUNCTION(BlueprintCallable)
	ABWWeaponFirearm* GetHoldedWeapon() const;

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
	void SetCanRun(bool Value);

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

	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CanInteract() const;

	UFUNCTION(BlueprintCallable)
	bool IsReloading() const;
	UFUNCTION(BlueprintCallable)
	void SetIsReloading(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CanReload() const;
	UFUNCTION(BlueprintCallable)
	void SetCanReload(bool Value);
	UFUNCTION(BlueprintCallable)
	bool IsHoldingWeapon() const;

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

	UFUNCTION()
	void LostHealth();
	UFUNCTION()
	void GainedHealth();
	UFUNCTION()
	void Death();

	UFUNCTION()
	void LostStamina();
	UFUNCTION()
	void GainedStamina();
	UFUNCTION()
	void StaminaEmptied();

	UFUNCTION()
	void LostShield();
	UFUNCTION()
	void GainedShield();
	UFUNCTION()
	void ShieldEmptied();
	
	void UpdateCharacterData() const;
	void InitAttributes();
};
