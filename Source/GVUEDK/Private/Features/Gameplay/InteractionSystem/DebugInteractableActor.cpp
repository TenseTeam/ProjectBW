// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/InteractionSystem/DebugInteractableActor.h"


ADebugInteractableActor::ADebugInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADebugInteractableActor::Interact_Implementation()
{
	FGvDebug::Warning("Cpp Interact", true);
}

