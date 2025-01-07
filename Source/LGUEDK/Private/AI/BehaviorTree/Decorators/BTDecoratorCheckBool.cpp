// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecoratorCheckBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecoratorCheckBool::UBTDecoratorCheckBool(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Bool";
}

bool UBTDecoratorCheckBool::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(BoolKey.SelectedKeyName);
	
}
