// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetNavMeshPoint.h"

#include "AIController.h"
#include "BTTask_FindNavMeshPoint.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	FVector ForwardVector = ControlledPawn->GetActorForwardVector();
	FVector PawnLocation = ControlledPawn->GetActorLocation();

	FVector TargetLocation;
	bool bFoundValidPoint = false;

	int MaxAttempts = 10; 
	int AttemptCount = 0;

	while (!bFoundValidPoint && AttemptCount < MaxAttempts)
	{
		AttemptCount++;
		FVector TestPoint = EQS_Manager->GetPoint(EnemyType); 

		FVector DirectionToPoint = (TestPoint - PawnLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(ForwardVector, DirectionToPoint);
		
		if (DotProduct <= 0)
		{
			if (IsPointFree(TestPoint, 250.0f, ControlledPawn)) 
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


bool UBTTask_GetNavMeshPoint::IsPointFree(FVector Point, float Radius, AActor* IgnoreActor)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(IgnoreActor); 
	QueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); 

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