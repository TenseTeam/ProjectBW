// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetFocus::UBTTask_SetFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTarget.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
