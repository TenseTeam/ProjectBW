// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNavMeshPoint.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_FindNavMeshPoint : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	
	explicit UBTTask_FindNavMeshPoint(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "Task Settings")
	bool bSamePlane;

	FVector FindFarthestPointWithinRange(const FVector& PlayerLocation,const FVector& EnemyLocation,float MinDistance, float MaxDistance);
};
