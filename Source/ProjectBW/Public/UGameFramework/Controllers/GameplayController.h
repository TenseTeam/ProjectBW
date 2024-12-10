// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/BWControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Features/Player/BWCharacter.h"
#include "GameplayController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API AGameplayController : public ABWControllerBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
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

	void AddMappingContext() const;
	
	void Walk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	//void Dodge(const FInputActionValue& Value);
	//void Jump(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);
	//void Shoot(const FInputActionValue& Value);
	//void StopShoot(const FInputActionValue& Value);
	//void Aim(const FInputActionValue& Value);
	//void StopAim(const FInputActionValue& Value);
	//void ThrowHook(const FInputActionValue& Value);
};
