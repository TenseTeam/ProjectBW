// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToRange.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_MoveToRange : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	explicit UBTTask_MoveToRange(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AcceptableRadiusKey;
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};

