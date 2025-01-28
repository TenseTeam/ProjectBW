// Copyright Villains, Inc. All Rights Reserved.


#include "GVUEDK/Public/Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"

#include <string>

#include "Features/Gameplay/GrapplingHookSystem/Behaviours/MovementBehaviours/Base/GHMovementModeBase.h"
#include "Features/Gameplay/GrapplingHookSystem/Behaviours/SearchBehaviours/Base/GHSearchModeBase.h"
#include "GameFramework/Character.h"
#include "Utility/FGvDebug.h"






UGrapplingHookComponent::UGrapplingHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTargetAcquired = false;
	bInitialized = false;
	TargetGrabPoint = nullptr;
	OwnerCharacter = nullptr;
	CurrentMovementMode = nullptr;
	CurrentSearchMode = nullptr;
}

void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
	{
		FGvDebug::Error(GetOwner()->GetName() + " is not a character, " + GetName() + " need to be on a character to work properly", true);
		bInitialized = false;
		return;
	}
	bInitialized = InitializeModes();
}

void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	if (!bInitialized)
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CurrentMovementMode->TickMode(DeltaTime))
	{
		return;
	}
	CurrentSearchMode->TickMode(DeltaTime);
}

void UGrapplingHookComponent::StartHooking()
{
	CurrentMovementMode->StartHooking();
}

void UGrapplingHookComponent::StopHooking()
{
	CurrentMovementMode->StopHooking();
}

float UGrapplingHookComponent::GetMaxDistance() const
{
	return CurrentSearchMode->GetMaxDistance();
}

float UGrapplingHookComponent::GetMinDistance() const
{	
	return CurrentSearchMode->GetMinDistance();
}

float UGrapplingHookComponent::GetTotalHookDistance() const
{	
	return CurrentMovementMode->GetTotalHookDistance();
}

float UGrapplingHookComponent::GetElapsedTime() const
{
	return CurrentMovementMode->GetElapsedTime();
}

float UGrapplingHookComponent::GetStartDelay() const
{
	return CurrentMovementMode->GetStartDelay();
}

FVector UGrapplingHookComponent::GetStartLocation() const
{
	if (IsValid(CurrentMovementMode))
		return CurrentMovementMode->GetStartLocation();
	return FVector::ZeroVector;
}

FVector UGrapplingHookComponent::GetLandingPointLocation() const
{
	return CurrentMovementMode->GetLandingPointLocation();
}

FVector UGrapplingHookComponent::GetStartDirection() const
{
	return CurrentMovementMode->GetStartDirection();
}



void UGrapplingHookComponent::ChangeMovementMode(int32 ModeIndex)
{
	if (ModeIndex < 0 || ModeIndex >= MovementModes.Num())
	{
		FGvDebug::Warning("Invalid Movement Mode Index");
		return;
	}
	if (IsValid(MovementModes[ModeIndex]))
	{
		CurrentMovementMode->ExitMode();
		CurrentMovementMode = MovementModes[ModeIndex];
		CurrentMovementMode->EnterMode();
	}
	else FGvDebug::Warning("Invalid Movement Mode at index " + FString::FromInt(ModeIndex));
}

void UGrapplingHookComponent::ChangeSearchMode(int32 ModeIndex)
{
	if (ModeIndex < 0 || ModeIndex >= SearchModes.Num())
	{
		FGvDebug::Warning("Invalid Search Mode Index");
		return;
	}
	if (IsValid(SearchModes[ModeIndex]))
	{
		CurrentSearchMode->ExitMode();
		CurrentSearchMode = SearchModes[ModeIndex];
		CurrentSearchMode->EnterMode();
	}
	else FGvDebug::Warning("Invalid Search Mode at index " + FString::FromInt(ModeIndex));
}

bool UGrapplingHookComponent::InitializeModes()
{
	if (MovementModes.Num() == 0)
	{
		FGvDebug::Warning(GetOwner()->GetName() + " Grappling Hook has no movement modes");
		return false;
	}
	if (SearchModes.Num() == 0)
	{
		FGvDebug::Warning(GetOwner()->GetName() + " Grappling Hook has no search modes");
		return false;
	}
	for (const auto Element : MovementModes)
	{
		if (IsValid(Element))
			Element->Initialize(this);
	}
	for (const auto Element : SearchModes)
	{
		if (IsValid(Element))
			Element->Initialize(this);
	}
	
	if (IsValid(MovementModes[0]))
		CurrentMovementMode = MovementModes[0];
	else
	{
		FGvDebug::Warning("Invalid Movement Mode at index 0");
		return false;
	}
	
	if (IsValid(SearchModes[0]))
		CurrentSearchMode = SearchModes[0];
	else
	{
		FGvDebug::Warning("Invalid Search Mode at index 0");
		return false;
	}
	
	return true;
}
