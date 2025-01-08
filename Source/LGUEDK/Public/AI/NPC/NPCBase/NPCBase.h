// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

UCLASS()
class LGUEDK_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCBase();

	UBehaviorTree* GetBehaviorTree() const;
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;
};
