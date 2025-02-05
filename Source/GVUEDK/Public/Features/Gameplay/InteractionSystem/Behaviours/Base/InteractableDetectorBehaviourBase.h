// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InteractionSystem/Interfaces/Interactable.h"
#include "UObject/Object.h"
#include "InteractableDetectorBehaviourBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class GVUEDK_API UInteractableDetectorBehaviourBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AActor* Owner;

public:
	UInteractableDetectorBehaviourBase();
	void Initialize(AActor* InOwner);
	virtual bool TryGetInteractable(TScriptInterface<IInteractable>& OutInteractable) const PURE_VIRTUAL(UInteractableDetectorBehaviourBase::TryGetInteractable, return false;);
};
