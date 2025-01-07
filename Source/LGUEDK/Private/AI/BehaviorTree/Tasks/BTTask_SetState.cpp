// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetState.h"

#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_SetState::UBTTask_SetState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set state";
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	ANPCBaseEnemy* ControlledPawn = Cast<ANPCBaseEnemy>(AIController->GetPawn());
	
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	ControlledPawn->SetEnemyState(DesiredState);
	
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	
	BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8 (DesiredState));
	
	LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)DesiredState).ToString(),true);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
