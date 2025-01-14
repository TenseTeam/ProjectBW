// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_CustomMoveTo.generated.h"

UCLASS()
class LGUEDK_API UBTTask_CustomMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:

	explicit UBTTask_CustomMoveTo(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	bool FindReachablePoint(UBehaviorTreeComponent& OwnerComp, FVector& OutReachableLocation) const;

};