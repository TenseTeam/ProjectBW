// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_IsWithInRange.h"

#include <string>

#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTDecorator_IsWithInRange::UBTDecorator_IsWithInRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is With In Range";
}

bool UBTDecorator_IsWithInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const FVector SelfPosition = BlackboardComp->GetValueAsVector(SelfPositionKey.SelectedKeyName);
	const FVector TargetPosition = BlackboardComp->GetValueAsVector(AttackTargetPositionKey.SelectedKeyName);
	
	const float DistanceFromPlayer = FVector::Dist(SelfPosition, TargetPosition);
	
	const float MaxRange = BlackboardComp->GetValueAsFloat(AccettableRadiusKey.SelectedKeyName);
	
	LGDebug::Log(FString::SanitizeFloat(DistanceFromPlayer), true);
	
	return DistanceFromPlayer <= MaxRange;
}
