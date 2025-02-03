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

	
	EEnemyType EnemyType = ControlledPawn->GetEnemyType();
	float Margin;
	
	switch (EnemyType) {
	case EEnemyType::Melee:
		Margin =  0.3f;
		break;
	case EEnemyType::Ranged:
		Margin = 0;
		break;
    default:
    	Margin =   0;
	}
	
	FVector PawnLocation = ControlledPawn->GetActorLocation();
	
	FVector TargetLocation;
	bool bFoundValidPoint = false;

	int MaxAttempts = 10; 
	int AttemptCount = 0;

	//calcolo direzione tra player e nemico
	//punto direzione punto e player
	// dot product tra direzione e punto

	
	while (!bFoundValidPoint && AttemptCount < MaxAttempts)
	{
		AttemptCount++;
		FVector TestPoint = EQS_Manager->GetPoint(EnemyType);
		
		FVector PlayerPosition = (ControlledPawn->GetAttackTarget()->GetActorLocation());
		FVector DirectionControlledPawnPlayer = (PawnLocation - PlayerPosition).GetSafeNormal();
		FVector DirectionToPoint = (TestPoint - PlayerPosition).GetSafeNormal();
		
		float DotProduct = FVector::DotProduct(DirectionControlledPawnPlayer, DirectionToPoint);
		
		if (DotProduct >= Margin)
		{
			if (IsPointFree(TestPoint, 200.0f, ControlledPawn)) 
			{
				TargetLocation = TestPoint;
				bFoundValidPoint = true;
			}
		}
	}
	
	if (!bFoundValidPoint)
	{
		FVector TestPoint = EQS_Manager->GetPoint(EnemyType);
		TargetLocation = TestPoint;
		UE_LOG(LogTemp, Warning, TEXT("Nessun punto valido trovato dopo %d tentativi"), MaxAttempts);
	}
	
	//DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, FColor::Red, false, 2.0f);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName,TargetLocation);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}


bool UBTTask_GetNavMeshPoint::IsPointFree(FVector Point, float Radius, ANPCBaseStateEnemy* ControlledPawn)
{
	AActor* AttackTarget = ControlledPawn->GetAttackTarget();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn); 
	QueryParams.AddIgnoredActor(AttackTarget); 

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, 
		Point, 
		Point + FVector(0, 0, 10), 
		FQuat::Identity, 
		ECC_Pawn, 
		FCollisionShape::MakeSphere(Radius), 
		QueryParams
	);

	bool bPointIsFree = !bHit || HitResults.Num() == 1; 

	FColor SphereColor = bPointIsFree ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Point, Radius, 12, SphereColor, false, 2.0f);
	

	return bPointIsFree;
}