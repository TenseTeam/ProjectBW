// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_CustomMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"

UBTTask_CustomMoveTo::UBTTask_CustomMoveTo(const FObjectInitializer& ObjectInitializer)
{
    NodeName = "Custom Move To ";
}

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }
    
    FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
    
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        return EBTNodeResult::Failed;
    }
    
    FVector ReachableLocation = TargetLocation;
    if (!FindReachablePoint(OwnerComp, ReachableLocation))
    {
        
        if (ReachableLocation == TargetLocation)
        {
            return EBTNodeResult::Failed; 
        }
    }

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}

bool UBTTask_CustomMoveTo::FindReachablePoint(UBehaviorTreeComponent& OwnerComp, FVector& OutReachableLocation) const
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
    if (!NavSys)
    {
        return false;
    }

    FNavLocation NavLocation;
    bool bFoundPath = NavSys->ProjectPointToNavigation(OutReachableLocation, NavLocation, FVector(500.0f, 500.0f, 500.0f));
    if (bFoundPath)
    {
        OutReachableLocation = NavLocation.Location + FVector(0.0f, 0.0f, 50.0f);
        return true;
    }

    return false;
}