// Copyright Villains, Inc. All Rights Reserved.


#include "Patterns/State/StateBase.h"

#include "Utility/FGvDebug.h"

void UStateBase::Initialize(AActor* Context)
{
}

void UStateBase::OnEnter_Implementation(AActor* Context)
{
}

void UStateBase::OnUpdate_Implementation(AActor* Context, float DeltaTime)
{
}

void UStateBase::OnExit_Implementation(AActor* Context)
{
}

void UStateBase::OnHandleInput_Implementation(AActor* Context, const EInputActionType InputAction,
                                              const FInputActionValue& Value)
{
}

void UStateBase::Enter(AActor* Context)
{
	OnEnter(Context);
}

void UStateBase::Update(AActor* Context, float DeltaTime)
{
	OnUpdate(Context, DeltaTime);
}

void UStateBase::Exit(AActor* Context)
{
	OnExit(Context);
}

void UStateBase::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	OnHandleInput(Context, InputAction, Value);
}
