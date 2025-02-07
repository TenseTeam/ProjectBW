// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

UCLASS()
class LGUEDK_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCBase();

	UFUNCTION(Blueprintable,BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() const;
	
	// UFUNCTION(Blueprintable,BlueprintCallable)
	// ANPCBaseController* GetAIController() const { return MyController; }
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
	// UPROPERTY()
	// ANPCBaseController* MyController;
	
	virtual void BeginPlay() override;
	
};
