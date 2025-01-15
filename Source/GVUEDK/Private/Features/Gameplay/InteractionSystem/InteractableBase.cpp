// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InteractionSystem/InteractableBase.h"


// Sets default values
AInteractableBase::AInteractableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AInteractableBase::CanBeInteracted_Implementation(AActor* Caller) const
{
	if (!bCanBeInteracted) return false;
	if (bObstacleInBetweenAllowed) return true;
	return true;
	
}

FVector AInteractableBase::GetInteractableLocation_Implementation() const
{
	return IInteractable::GetInteractableLocation_Implementation();
}

