// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetMovementSpeed.h"

#include "AI/EnemyBase/EnemyBase.h"
#include "AI/EnemyBase/EnemyControllerBase.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set movement speed";
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Ottieni il controller AI
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}
	
	Enemy->SetMovementSpeed(MovementSpeed);

	return EBTNodeResult::Succeeded;
}
