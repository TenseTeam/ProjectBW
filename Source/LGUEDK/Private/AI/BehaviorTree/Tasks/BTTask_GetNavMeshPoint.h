// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNavMeshPoint.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetNavMeshPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_GetNavMeshPoint(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	bool IsPointFree(FVector Point, float Radius, AActor* IgnoreActor);
};
