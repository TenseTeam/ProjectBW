// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/MotionStates/Hook.h"

#include <string>

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void UHook::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	PlayerGrapplingHook = Cast<UGrapplingHookComponent>(Character->GetComponentByClass(UGrapplingHookComponent::StaticClass()));
	if (!IsValid(PlayerGrapplingHook))
	{
		FGvDebug::Warning(GetName() + " failed to initialize, missing GrapplingHookComponent ref", true);
		bInitialized = false;
		return;
	}
	if (!IsValid(Character->Data))
	{
		FGvDebug::Warning(GetName() + " failed to initialize, missing CharacterData ref", true);
		bInitialized = false;
	}
}

void UHook::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);
	
	Character->MotionState = ECharacterState::Hooking;
	Character->HookState = EHookState::HookStart;
	
	PlayerGrapplingHook->OnStartHooking.AddDynamic(this, &UHook::OnHookStarted);
	PlayerGrapplingHook->OnStopHooking.AddDynamic(this, &UHook::OnHookFinished);
	PlayerGrapplingHook->OnHookMotionStarted.AddDynamic(this, &UHook::OnHookMotionStarted);
	PlayerGrapplingHook->StartHooking();
}

void UHook::Update(AActor* Context, float deltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, deltaTime);

	if (ElapsedTime < StartDelay)
	{
		ElapsedTime += deltaTime;
		return;
	}
	Character->HookState = EHookState::HookLoop;
	// const float SquaredTotalDistance = PlayerGrapplingHook->GetTotalHookDistance() * PlayerGrapplingHook->GetTotalHookDistance();
	// const float SquaredElapsedDistance = FVector::DistSquared(Character->GetActorLocation(), PlayerGrapplingHook->GetStartLocation());
	// const float NormalizedElapsedDistance = SquaredElapsedDistance / SquaredTotalDistance;
	// if (NormalizedElapsedDistance < HookEndNormalizedDistance)
	// {
	// 	Character->HookState = EHookState::HookLoop;
	// }
	// else
	// {
	// 	Character->HookState = EHookState::HookEnd;
	// }
}

void UHook::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	PlayerGrapplingHook->OnStartHooking.RemoveDynamic(this, &UHook::OnHookStarted);
	PlayerGrapplingHook->OnStopHooking.RemoveDynamic(this, &UHook::OnHookFinished);
	PlayerGrapplingHook->OnHookMotionStarted.RemoveDynamic(this, &UHook::OnHookMotionStarted);
	Character->HookState = EHookState::None;
}

void UHook::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);
}

void UHook::OnHookFinished() 
{
	Character->GetSpringArm()->bDoCollisionTest = true;
	Character->SetIsHooking(false);
	Character->SetActorEnableCollision(true);
	
	if (Character->GetGroundDistance() > 100)
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
}

void UHook::OnHookStarted()
{
	//HookEndNormalizedDistance = Character->Data->HookEndNormalizedDistance;
	StartDelay = PlayerGrapplingHook->GetStartDelay();
	ElapsedTime = 0;
	Character->SetIsHooking(true);
	Character->GetCharacterMovement()->bNotifyApex = true;
}

void UHook::OnHookMotionStarted()
{
	Character->GetSpringArm()->bDoCollisionTest = false;
	Character->SetActorEnableCollision(false);
}
