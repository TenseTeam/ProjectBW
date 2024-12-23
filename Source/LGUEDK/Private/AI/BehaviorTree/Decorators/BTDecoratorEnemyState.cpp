// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecoratorEnemyState.h"

#include "AI/EnemyBase/EnemyBase.h"

UBTDecoratorEnemyState::UBTDecoratorEnemyState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Enemy State";
}

bool UBTDecoratorEnemyState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	const AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if (!Enemy)
	{
		return false;
	}
	
	return Enemy->GetEnemyState() == RequiredState;
}
