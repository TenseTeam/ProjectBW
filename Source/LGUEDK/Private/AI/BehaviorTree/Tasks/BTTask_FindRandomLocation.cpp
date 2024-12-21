// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "AI/EnemyBase/EnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (AEnemyControllerBase* const controller = Cast<AEnemyControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (APawn* const enemy = controller->GetPawn())
		{
			//Get enemy initial location to use as origin
			FVector const initialPosition = enemy->GetActorLocation();

			if (UNavigationSystemV1* const navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation location;
				if (navigationSystem->GetRandomReachablePointInRadius(initialPosition,SearchRadius,location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), location.Location);
				}

				//finish with success
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
