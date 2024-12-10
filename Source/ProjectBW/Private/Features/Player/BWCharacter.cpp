// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/BWCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "UGameFramework/Controllers/GameplayController.h"
#include "Utility/FGvDebug.h"


ABWCharacter::ABWCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm = CreateDefaultSubobject<UGvSpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>("StateMachineComponent");

	bCanMove = true;
	bCanLook = true;
	bCanRun = true;
}

void ABWCharacter::BeginPlay()
{
	Super::BeginPlay();
	BWController = Cast<AGameplayController>(GetController());
}

void ABWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABWCharacter::HandleInput(const EInputActionType InputAction, const FInputActionValue& Value)
{
	StateMachineComponent->HandleInput(InputAction, Value);
}

void ABWCharacter::ChangeState(const int Index) const
{
	StateMachineComponent->ChangeState(Index);
}

const UCharacterStatsBase* ABWCharacter::GetState(const int Index) const
{
	return Cast<UCharacterStatsBase>(StateMachineComponent->GetState(Index));
}

void ABWCharacter::Move(const FVector& MoveVector)
{
	FRotator CameraCurrentRotation = BWController->PlayerCameraManager->GetCameraCacheView().Rotation;
	FRotator MoveVectorRotationOffset(0, CameraCurrentRotation.Yaw, 0);
	AddMovementInput(MoveVectorRotationOffset.Quaternion() * MoveVector);
}

bool ABWCharacter::IsRunning() const
{
	return bIsRunning && !AGameplayController::GetMoveInputValue().IsNearlyZero();
}

void ABWCharacter::SetIsRunning(bool Value)
{
	bIsRunning = Value;
}

bool ABWCharacter::CanMove() const
{
	return bCanMove;
}

bool ABWCharacter::CanLook() const
{
	return bCanLook;
}
 
bool ABWCharacter::CanRun() const
{
	return bCanRun;
}




