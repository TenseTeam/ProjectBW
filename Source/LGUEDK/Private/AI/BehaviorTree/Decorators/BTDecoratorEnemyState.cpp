// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecoratorEnemyState.h"
#include "AIController.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"

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

	const ANPCBaseEnemy* Enemy = Cast<ANPCBaseEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return false;
	}
	
	return Enemy->GetState() == RequiredState;
}


