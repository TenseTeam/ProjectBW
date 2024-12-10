// Copyright Villains, Inc. All Rights Reserved.


#include "Patterns/State/StateMachineComponent.h"

#include "Utility/FGvDebug.h"


UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bInitialized = false;
}

void UStateMachineComponent::Initialize()
{
	//Context initialization
	if (bUseExternalContext && IsValid(ContextClass))
	{
		Context = NewObject<AActor>(this, ContextClass);
	}
	else if (!bUseExternalContext && IsValid(GetOwner()))
	{
		Context = GetOwner();
	}
	else
	{
		FGvDebug::Error("Invalid Context");
		Context = nullptr;
		bInitialized = false;
		return;
	}

	//States initialization
	if (StateClasses.Num() > 0)
	{
		FGvDebug::Log("Initializing States");
		States = TArray<UStateBase*>();
		for (const auto StateClass : StateClasses)
		{
			if (IsValid(StateClass))
			{
				UStateBase* State = NewObject<UStateBase>(this, StateClass);
				State->Initialize(Context);
				States.Emplace(State);
				FGvDebug::Log("State " + State->GetName() + " created");
				continue;
			}
			FGvDebug::Warning("Invalid State Class");
		}
	}

	if (States.Num() > 0)
	{
		CurrentState = States[0];
		bInitialized = true;
	}
	else
	{
		bInitialized = false;
	}
}

void UStateMachineComponent::ChangeState(const int Index)
{
	if (bInitialized)
	{
		if (CurrentState == States[Index])
		{
			FGvDebug::Warning("State already running");
			return;
		}
		if (States.Num() > Index)
		{
			PreviousState = CurrentState;
			CurrentState->Exit(Context);
			CurrentState = States[Index];
			CurrentState->Enter(Context);
		}
	}
}

UStateBase* UStateMachineComponent::GetState(const int Index)
{
	if (bInitialized)
	{
		if (States.Num() > Index)
		{
			return States[Index];
		}
		else
		{
			FGvDebug::Warning("Invalid Index");
			return nullptr;
		}
	}
	FGvDebug::Warning("Component not initialized");
	return nullptr;
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
	if (bInitialized)
	{
		CurrentState->Enter(Context);
	}
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bInitialized)
	{
		CurrentState->Update(Context, DeltaTime);
	}
}

void UStateMachineComponent::HandleInput(const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (bInitialized)
	{
		CurrentState->HandleInput(Context, InputAction, Value);
	}
}

