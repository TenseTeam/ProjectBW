// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Hook.h"

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
	
	HookEndNormalizedDistance = Character->Data->HookEndNormalizedDistance;
	StartDelay = PlayerGrapplingHook->GetStartDelay();
	ElapsedTime = 0;
	
	Character->CharacterState = ECharacterState::Hooking;
	Character->HookState = EHookState::HookStart;
	Character->SetIsHooking(true);
	Character->GetCharacterMovement()->bNotifyApex = true;
	Character->GetCharacterMovement()->GravityScale = 0;
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->SetActorEnableCollision(false);
	
	PlayerGrapplingHook->OnStopHooking.AddDynamic(this, &UHook::OnHookFinished);
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

	const float SquaredTotalDistance = PlayerGrapplingHook->GetTotalHookDistance() * PlayerGrapplingHook->GetTotalHookDistance();
	const float SquaredElapsedDistance = FVector::DistSquared(Character->GetActorLocation(), PlayerGrapplingHook->GetStartLocation());
	const float NormalizedElapsedDistance = SquaredElapsedDistance / SquaredTotalDistance;
	if (NormalizedElapsedDistance < HookEndNormalizedDistance)
	{
		Character->GetSpringArm()->bDoCollisionTest = false;
		Character->HookState = EHookState::HookLoop;
	}
	else
	{
		Character->HookState = EHookState::HookEnd;
	}
}

void UHook::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	PlayerGrapplingHook->OnStopHooking.RemoveDynamic(this, &UHook::OnHookFinished);
	Character->HookState = EHookState::None;
	Character->GetSpringArm()->bDoCollisionTest = true;
	Character->SetIsHooking(false);
	Character->GetCharacterMovement()->GravityScale = 1;
	Character->SetActorEnableCollision(true);
}

void UHook::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);
}

void UHook::OnHookFinished() 
{
	if (Controller->GetMoveInputValue().IsNearlyZero())
	{
		Character->ChangeState(0); //idle
	}
	else
	{
		Character->ChangeState(1); //walk
	}
}
