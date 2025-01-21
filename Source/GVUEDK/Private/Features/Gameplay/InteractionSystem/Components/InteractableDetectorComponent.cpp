// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InteractionSystem/Components/InteractableDetectorComponent.h"

#include "Features/Gameplay/InteractionSystem/Behaviours/Base/InteractableDetectorBehaviourBase.h"
#include "Utility/FGvDebug.h"

UInteractableDetectorComponent::UInteractableDetectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractableDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(DetectorBehaviour))
	{
		FGvDebug::Error(GetName() + " Behaviour is not valid", true);
		return;
	}
	DetectorBehaviour->Initialize(GetOwner());
}

void UInteractableDetectorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckInteractablesInRange();
}

bool UInteractableDetectorComponent::TryGetInteractable(TScriptInterface<IInteractable>& OutInteractable) const
{
	return DetectorBehaviour->TryGetInteractable(OutInteractable);
}

void UInteractableDetectorComponent::CheckInteractablesInRange()
{
	TScriptInterface<IInteractable> OutInteractable = nullptr;
	if (TryGetInteractable(OutInteractable))
	{
		if (OutInteractable.GetObject() != LastDetectedInteractable.GetObject())
		{
			if (IsValid(LastDetectedInteractable.GetObject()))
				IInteractable::Execute_Unhighlight(LastDetectedInteractable.GetObject());
			
			LastDetectedInteractable = OutInteractable;
			IInteractable::Execute_Highlight(OutInteractable.GetObject());
			
			OnInteractableDetected.Broadcast(OutInteractable);
		}
	}
	else if (LastDetectedInteractable.GetObject() != nullptr)
	{
		IInteractable::Execute_Unhighlight(LastDetectedInteractable.GetObject());
		LastDetectedInteractable = nullptr;
		OnInteractableLost.Broadcast();
	}
}




