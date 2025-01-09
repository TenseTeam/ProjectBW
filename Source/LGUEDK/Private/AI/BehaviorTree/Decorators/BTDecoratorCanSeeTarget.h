// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecoratorCanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecoratorCanSeeTarget : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

protected:
	explicit UBTDecoratorCanSeeTarget(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Raycast")
	bool bDebugLineTrace = false;
	
	UPROPERTY(EditAnywhere, Category = "Raycast")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
};
