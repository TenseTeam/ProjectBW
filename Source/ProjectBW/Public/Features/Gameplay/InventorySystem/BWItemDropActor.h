// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InteractionSystem/Interfaces/Interactable.h"
#include "Features/Gameplay/InventorySystem/Base/ItemDropActor.h"
#include "BWItemDropActor.generated.h"

class URPGInventory;

UCLASS()
class PROJECTBW_API ABWItemDropActor : public AItemDropActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY()
	URPGInventory* PlayerInventory;

public:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation() override;
	virtual bool CanBeInteracted_Implementation(AActor* Caller) const override;
	virtual FVector GetInteractableLocation_Implementation() const override;
};
