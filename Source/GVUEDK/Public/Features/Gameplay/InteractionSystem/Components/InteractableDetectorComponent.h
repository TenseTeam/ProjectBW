// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableDetectorComponent.generated.h"


class UInteractableDetectorBehaviourBase;
class IInteractable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UInteractableDetectorComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta = (AllowPrivateAccess = "true"))
	UInteractableDetectorBehaviourBase* DetectorBehaviour;

public:
	UInteractableDetectorComponent();
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool TryGetInteractable(TScriptInterface<IInteractable>& OutInteractable) const;

};
