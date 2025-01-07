// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_ClearFocus.h"
#include "AIController.h"

UBTTask_ClearFocus::UBTTask_ClearFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearEnemyFocusTask: AIController is null"));
		return EBTNodeResult::Failed;
	}
	
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	UE_LOG(LogTemp, Log, TEXT("ClearEnemyFocusTask: Focus cleared"));

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
