// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ActionState.h"
#include "Interact.generated.h"

class IInteractable;
/**
 * 
 */
UCLASS()
class PROJECTBW_API UInteract : public UActionState
{
	GENERATED_BODY()

	TScriptInterface<IInteractable> Interactable;
	virtual void Enter(AActor* Context) override;
};
