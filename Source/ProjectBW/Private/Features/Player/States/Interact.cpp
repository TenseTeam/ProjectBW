// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Interact.h"

#include "Features/Gameplay/InteractionSystem/Components/InteractableDetectorComponent.h"
#include "Features/Gameplay/InteractionSystem/Interfaces/Interactable.h"
#include "Utility/FGvDebug.h"

void UInteract::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	if (Character->GetInteractableDetector()->TryGetInteractable(Interactable))
	{
		IInteractable::Execute_Interact(Interactable.GetObject());
		Character->OnInteract.Broadcast(Cast<AActor>(Interactable.GetObject()));
	}
	
	if (Character->WantShooting() || Character->WantAiming())
	{
		Character->ChangeActionState(1);
		return;
	}
	Character->ChangeActionState(0);
}
