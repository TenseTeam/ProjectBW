// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecoratorCheckBool.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecoratorCheckBool : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	explicit UBTDecoratorCheckBool(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	FBlackboardKeySelector BoolKey;

};
