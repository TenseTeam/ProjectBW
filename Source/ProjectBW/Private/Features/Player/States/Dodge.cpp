// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Dodge.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void UDodge::Initialize(AActor* Context)
{
	Super::Initialize(Context);

	if (!IsValid(Character->Data) || !IsValid(Character->Data->DodgeAnimMontage))
	{
		FGvDebug::Warning("Dodge state has missing required data", true);
		bInitialized = false;
		return;
	}

	DefaultBrakingDecelerationWalking = Character->GetCharacterMovement()->BrakingDecelerationWalking;
	DodgeAnimMontage = Character->Data->DodgeAnimMontage;
	DodgeAnimLenght = DodgeAnimMontage->GetPlayLength() / Character->Data->DodgeAnimPlayRate;
	//FGvDebug::Log("Lenght: " + FString::SanitizeFloat(DodgeAnimMontage->GetPlayLength()), true);
	//FGvDebug::Log("Rate: " + FString::SanitizeFloat(Character->Data->DodgeAnimPlayRate), true);
	//FGvDebug::Log("DodgeAnimLenght: " + FString::SanitizeFloat(DodgeAnimLenght), true);
	ExitNormalizedTime = Character->Data->ExitNormalizedTime;
	RotationSpeed = Character->Data->RotationSpeed;
}

void UDodge::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	Character->MotionState = ECharacterState::Dodging;
	Character->SetIsDodging(true);
	
	Character->GetCharacterMovement()->BrakingDecelerationWalking = 0;
	ElapsedTime = 0.f;
	DodgeSpeed = Character->Data->DodgeSpeed;
	DodgeCooldown = Character->Data->DodgeCooldown;
	
	TargetVelocity = GetRollVelocity();
	TargetRotation = TargetVelocity.Rotation();
	TargetRotation.Pitch = 0;
	TargetRotation.Roll = 0;

	CurrentRotation = Character->GetActorRotation();
	
	Character->PlayAnimMontage(DodgeAnimMontage, Character->Data->DodgeAnimPlayRate);
	Character->OnStartDodging.Broadcast();

	StartCooldown();
}

void UDodge::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);

	//if exit dodge time is reached we want to change the state
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= DodgeAnimLenght * ExitNormalizedTime)
	{
		if (Character->GetCharacterMovement()->IsFalling())
		{
			Character->ChangeMotionState(3); //jump
			return;
		}

		if (Controller->GetMoveInputValue().IsNearlyZero())
		{
			Character->ChangeMotionState(0); //idle
			return;
		}
		
		Character->ChangeMotionState(1); //walk
		return;
	}

	//if the character is falling we want to apply gravity
	if (Character->GetCharacterMovement()->IsFalling())
	{
		Character->GetCharacterMovement()->Velocity = TargetVelocity + FVector::DownVector * 981;
	}
	else
	{
		Character->GetCharacterMovement()->Velocity = TargetVelocity;
	}

	CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	Character->SetActorRotation(CurrentRotation);
}

void UDodge::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->GetCharacterMovement()->BrakingDecelerationWalking = DefaultBrakingDecelerationWalking;
	Character->SetIsDodging(false);
	Character->OnStopDodging.Broadcast();
}

void UDodge::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);
}

FVector UDodge::GetRollVelocity() const
{
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		const FVector CharacterForward = Character->GetActorForwardVector();
		return CharacterForward * DodgeSpeed;
	}
	
	FRotator OffsetRotation = Character->GetControlRotation();
	OffsetRotation.Pitch = 0;
	OffsetRotation.Roll = 0;
	const FVector Direction = OffsetRotation.Quaternion() * Controller->GetMoveInputValue().GetSafeNormal();
	return Direction * DodgeSpeed;
}

void UDodge::StartCooldown() const
{
	if (const UWorld* World = Character->GetWorld())
	{
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { Character->SetCanDodge(true); });
		
		FTimerHandle TimerHandle;
		
		World->GetTimerManager().SetTimer(TimerHandle, TimerDel, DodgeCooldown, false);

		//if the DodgeCooldown <= 0 the timer does not start so we do not want bCanRoll to become false
		const bool CanDodge = !(DodgeCooldown > 0);
		Character->SetCanDodge(CanDodge);
	}
}
