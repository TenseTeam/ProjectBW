// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NPCBaseController.generated.h"

class ANPCBase;

UCLASS()
class LGUEDK_API ANPCBaseController : public AAIController
{
	GENERATED_BODY()

public:
	 ANPCBaseController();
	
protected:

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void InitializeBlackboardValues() {};

	ANPCBase* GetControlledPawn() const;
	
	virtual void BeginPlay() override;

	UPROPERTY()
	ANPCBase* ControlledPawn;

};


