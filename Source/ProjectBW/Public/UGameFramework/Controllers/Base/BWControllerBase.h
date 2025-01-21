// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Generic/IMCManagerSystem/ContextsManager.h"
#include "Features/UI/MenuSystem/MenuManager.h"
#include "GameFramework/PlayerController.h"
#include "BWControllerBase.generated.h"

UCLASS()
class PROJECTBW_API ABWControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UContextsManager* ContextsManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMenuManager* MenuManager;
	
public:
	ABWControllerBase();
};
