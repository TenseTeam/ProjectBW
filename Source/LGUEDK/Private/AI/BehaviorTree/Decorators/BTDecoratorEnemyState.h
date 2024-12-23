// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EnemyState.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecoratorEnemyState.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecoratorEnemyState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	explicit UBTDecoratorEnemyState(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyState RequiredState;
};
