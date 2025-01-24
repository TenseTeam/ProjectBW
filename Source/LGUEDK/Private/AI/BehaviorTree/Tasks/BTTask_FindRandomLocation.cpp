// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	 if (ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner()))
    {
        if (APawn* const Enemy = Controller->GetPawn())
        {
            bWantExplore = Cast<ANPCBaseStateEnemy>(Enemy)->GetWantExplore();
            FVector InitialSpawnPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InitialPositionKey.SelectedKeyName);
            FVector InitialPosition = Enemy->GetActorLocation();
            float SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);
            
            if (UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                FNavLocation Location;
                FVector BestLocation = InitialSpawnPosition;

                if (!bWantExplore)
                {
                    if (NavigationSystem->GetRandomReachablePointInRadius(InitialSpawnPosition, SearchRadius, Location))
                    {
                        BestLocation = Location.Location;
                    }
                }
                else
                {
                    if (NavigationSystem->GetRandomReachablePointInRadius(InitialPosition, SearchRadius, Location))
                    {
                        BestLocation = Location.Location;
                    }
                }

                // Imposta il punto selezionato sul Blackboard
                OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", BestLocation);

                
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    
    return EBTNodeResult::Failed;
}
