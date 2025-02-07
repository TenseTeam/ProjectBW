// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Features/Player/BWCharacter.h"
#include "Input/ActionManagementSystem/Components/ActionManagerComponent.h"
#include "GameplayController.generated.h"


class UCharacterStats;
class UEquipment;
/**
 * 
 */
UCLASS()
class PROJECTBW_API AGameplayController : public ABWControllerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBWInventory* Inventory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquipment* Equipment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCharacterStats* CharacterStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UActionManagerComponent* ActionManager;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowHookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY()
	ABWCharacter* BWCharacter;
	
	static FVector MoveInputValue;

public:
	AGameplayController();
	FORCEINLINE static FVector GetMoveInputValue() { return MoveInputValue; }

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void Look(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void ThrowHook(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StopShoot(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
};
