// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
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
            FVector InitialPosition = Enemy->GetActorLocation();
            
            float SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);
            
            if (UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                FNavLocation Location;
                FVector BestLocation = InitialPosition;

                if (bWantExplore)
                {
                    if (NavigationSystem->GetRandomReachablePointInRadius(InitialPosition, SearchRadius, Location))
                    {
                        BestLocation = Location.Location;
                    }
                }
                else
                {
                    if (NavigationSystem->GetRandomReachablePointInRadius(InitialPosition, SearchRadius, Location))
                    {
                        FVector CandidateLocation = Location.Location;
                        
                        float DistanceFromInitial = FVector::Distance(InitialPosition, CandidateLocation);
                        if (DistanceFromInitial <= SearchRadius)
                        {
                            BestLocation = CandidateLocation;
                        }
                        else
                        {
                            // Se nessun punto valido è trovato entro il raggio, usa un punto casuale vicino
                            NavigationSystem->GetRandomReachablePointInRadius(InitialPosition, SearchRadius, Location);
                            BestLocation = Location.Location;
                        }
                    }
                }

                // Imposta il punto selezionato sul Blackboard
                OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", BestLocation);

                // Concludi con successo
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
    }

    // Concludi con un fallimento se qualcosa va storto
    return EBTNodeResult::Failed;
}
