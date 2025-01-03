// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NPCBaseController.generated.h"

class NPCBase;

UCLASS()
class LGUEDK_API ANPCBaseController : public AAIController
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
};


