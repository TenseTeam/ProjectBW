// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetNavMeshPoint.h"

#include "AIController.h"
#include "BTTask_FindNavMeshPoint.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetNavMeshPoint::UBTTask_GetNavMeshPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "GetNavMeshPoint";
}

EBTNodeResult::Type UBTTask_GetNavMeshPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearEnemyFocusTask: AIController is null"));
		return EBTNodeResult::Failed;
	}
	
	ANPCBaseStateEnemy* ControlledPawn = Cast<ANPCBaseStateEnemy>(AIController->GetPawn());
	if (!ControlledPawn)return EBTNodeResult::Failed;
	
	AEQS_Manager* EQS_Manager = ControlledPawn->GetEQSManager();
	if (!EQS_Manager)return EBTNodeResult::Failed;
	
	EQS_Manager->CanSearchPoint = true;
	EEnemyType EnemyType = ControlledPawn->GetEnemyType();
	FVector TargetLocation = EQS_Manager->GetPoint(EnemyType);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName,TargetLocation);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
