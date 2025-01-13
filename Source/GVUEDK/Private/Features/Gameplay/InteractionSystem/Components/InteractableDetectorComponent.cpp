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

bool UInteractableDetectorComponent::TryGetInteractable(TScriptInterface<IInteractable>& OutInteractable) const
{
	return DetectorBehaviour->TryGetInteractable(OutInteractable);
}




