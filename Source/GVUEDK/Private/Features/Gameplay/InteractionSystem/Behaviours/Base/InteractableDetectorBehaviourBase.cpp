// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InteractionSystem/Behaviours/Base/InteractableDetectorBehaviourBase.h"

UInteractableDetectorBehaviourBase::UInteractableDetectorBehaviourBase() :
Owner(nullptr)
{
	
}

void UInteractableDetectorBehaviourBase::Initialize(AActor* InOwner)
{
	Owner = InOwner;
}
