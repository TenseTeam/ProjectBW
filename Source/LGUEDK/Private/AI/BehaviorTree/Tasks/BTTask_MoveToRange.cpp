// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_MoveToRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_MoveToRange::UBTTask_MoveToRange(FObjectInitializer const& ObjectInitializer)
{
}

EBTNodeResult::Type UBTTask_MoveToRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Otteniamo il BlackboardComponent associato
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed; // Fallisce se non c'è il Blackboard
	}

	// Otteniamo il controller AI
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed; // Fallisce se il controller AI non è disponibile
	}

	// Recuperiamo la posizione dell'AI e del target dal Blackboard
	const FVector SelfPosition = AIController->GetPawn()->GetActorLocation();
	const FVector TargetPosition = BlackboardComp->GetValueAsVector(TargetPositionKey.SelectedKeyName);

	// Calcoliamo la distanza tra l'AI e il target
	const float DistanceFromTarget = FVector::Dist(SelfPosition, TargetPosition);
	const float AcceptableRadius = BlackboardComp->GetValueAsFloat(AcceptableRadiusKey.SelectedKeyName);

	// Se siamo già in range, completiamo subito il task
	if (DistanceFromTarget <= AcceptableRadius)
	{
		return EBTNodeResult::Succeeded;
	}

	// Altrimenti, avviamo il movimento verso il target
	FAIMoveRequest MoveRequest(TargetPosition);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);
	MoveRequest.SetCanStrafe(true); // Consenti il movimento laterale se necessario

	FNavPathSharedPtr NavPath;
	EPathFollowingRequestResult::Type Result = AIController->MoveTo(MoveRequest, &NavPath);

	
	if (Result == EBTNodeResult::InProgress)
	{
		return EBTNodeResult::InProgress; 
	}
	
	return EBTNodeResult::Failed;
	
}
