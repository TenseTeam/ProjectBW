// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsWieldingASword.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UBTDecorator_IsWieldingASword : public UBTDecorator
{
	GENERATED_BODY()

protected:
	
	explicit UBTDecorator_IsWieldingASword(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
