// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindNavMeshPoint.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "NavMesh/RecastNavMesh.h"
#include "Utility/LGDebug.h"


UBTTask_FindNavMeshPoint::UBTTask_FindNavMeshPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find NavMesh Point";
	bSamePlane = true;
}

EBTNodeResult::Type UBTTask_FindNavMeshPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
	
    AActor* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) return EBTNodeResult::Failed;
	
    AActor* PlayerCharacter = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!PlayerCharacter) return EBTNodeResult::Failed;

    float MinDistance = BlackboardComp->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector ControlledPawnLocation = ControlledPawn->GetActorLocation();
    FVector TargetLocation = FindFarthestPointWithinRange(PlayerLocation,ControlledPawnLocation,MinDistance, MaxDistance);

    BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

    return EBTNodeResult::Succeeded;
}

FVector UBTTask_FindNavMeshPoint::FindFarthestPointWithinRange(const FVector& PlayerLocation,const FVector& EnemyLocation,float MinDistance, float MaxDistance)
{
    float DistanceToEnemy = FVector::Dist(PlayerLocation,EnemyLocation);
    if (DistanceToEnemy >= MinDistance && DistanceToEnemy <= MaxDistance) return EnemyLocation;
    
    FVector BestPoint = PlayerLocation;  
    float BestDistance = -1.0f;
    FVector PlayerNavMeshLocation = FVector::ZeroVector;
    
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem) return BestPoint;
    
    TArray<FNavLocation> NavPoints;
    FNavLocation NavLocation;
    
    if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, MaxDistance, NavLocation))
    {
        NavPoints.Add(NavLocation);
    }

    if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation,1, NavLocation))
    {
        PlayerNavMeshLocation = NavLocation.Location;
        LGDebug::Log("player location Z (World) : " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    }
    else LGDebug::Log("player location Z (World) NON TROVATO: " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    
    if (bSamePlane)
    {
        for (const FNavLocation& NavPoint : NavPoints)
        {
            float Distance = FVector::Dist(PlayerLocation, NavPoint.Location);

            if (Distance >= MinDistance && Distance <= MaxDistance)
            {
                if (NavPoint.Location.Z == PlayerNavMeshLocation.Z)
                {
                    LGDebug::Log("punto trovato z NavPoint.Location.Z" + FString::SanitizeFloat(NavPoint.Location.Z), true);
                    if (Distance > BestDistance)
                    {
                        BestDistance = Distance;
                        BestPoint = NavPoint.Location;
                    }
                }
            }
        }   
    }
    else
    {
        for (const FNavLocation& NavPoint : NavPoints)
        {
            float Distance = FVector::Dist(PlayerLocation, NavPoint.Location);
            if (Distance >= MinDistance && Distance <= MaxDistance)
            {
                if (Distance > BestDistance)
                {
                    BestDistance = Distance;
                    BestPoint = NavPoint.Location;
               }
            }
        }
    }
    
    if (BestDistance == -1.0f)
    {
        for (const FNavLocation& NavPoint : NavPoints)
        {
            float Distance = FVector::Dist(PlayerLocation, NavPoint.Location);
            if (Distance > BestDistance)
            {
                BestDistance = Distance;
                BestPoint = NavPoint.Location;
                LGDebug::Log("punto NON trovato z NavPoint.Location.Z" + FString::SanitizeFloat(BestPoint.Z), true);
            }
        }
    }
    
    return BestPoint;
}