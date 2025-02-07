// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTService_CheckDistance : public UBTService
{
	GENERATED_BODY()

public:
	
	explicit UBTService_CheckDistance(FObjectInitializer const& ObjectInitializer);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceFromPlayerKey;
	
private:
	UPROPERTY()
	UBlackboardComponent* CachedBlackboardComp;
};
