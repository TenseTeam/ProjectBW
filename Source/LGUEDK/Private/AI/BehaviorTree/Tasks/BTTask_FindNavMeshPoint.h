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

	FVector FindFarthestPointWithinRange(const AActor* Player,const AActor* Enemy,float MinDistance, float MaxDistance);

	bool IsHittingSomething(const FVector& Start, const FVector& End,const AActor* Enemy);

	bool IsInRange(float Distance,const float MinDistance,const float MaxDistance);

	FVector GetCorrectNavPoint(TArray<FNavLocation> NavPoints,float BestDistance, const FVector& PlayerLocation,FVector PlayerNavMeshLocation,const float MinDistance,const float MaxDistance,const AActor* Enemy);
};
