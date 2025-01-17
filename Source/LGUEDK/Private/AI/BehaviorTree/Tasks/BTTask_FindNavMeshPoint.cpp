// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindNavMeshPoint.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
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

    FVector TargetLocation = FindFarthestPointWithinRange(PlayerCharacter,ControlledPawn,MinDistance, MaxDistance);

    BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

    return EBTNodeResult::Succeeded;
}

FVector UBTTask_FindNavMeshPoint::FindFarthestPointWithinRange(const AActor* Player,const AActor* Enemy,float MinDistance, float MaxDistance)
{
    FVector PlayerLocation = Player->GetActorLocation();
    FVector EnemyLocation = Enemy->GetActorLocation();
    float DistanceToEnemy = FVector::Dist(PlayerLocation,EnemyLocation);
    if (IsInRange(DistanceToEnemy,MinDistance,MaxDistance)) return EnemyLocation;
    
    FVector BestPoint = PlayerLocation;  
    float BestDistance = -1.0f;
    FVector PlayerNavMeshLocation = FVector::ZeroVector;
    TArray<FNavLocation> NavPoints;
    FNavLocation NavLocation;

    
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem) return BestPoint;
    
    if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, MaxDistance, NavLocation))
    {
        NavPoints.Add(NavLocation);
    }
    
    if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation ,MaxDistance, NavLocation))
    {
        PlayerNavMeshLocation = NavLocation.Location;
        LGDebug::Log("player location Z (World) : " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    }
    else LGDebug::Log("player location Z (World) NON TROVATO: " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    
    if (bSamePlane)
    {
        BestPoint = GetCorrectNavPoint(NavPoints,BestDistance,PlayerLocation,PlayerNavMeshLocation,MinDistance,MaxDistance,Enemy); 
    }
    else
    {
        for (const FNavLocation& NavPoint : NavPoints)
        {
            float Distance = FVector::Distance(PlayerLocation, NavPoint.Location);
            if (IsInRange(Distance,MinDistance,MaxDistance) && !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy))
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
        BestPoint = GetCorrectNavPoint(NavPoints,BestDistance,PlayerLocation,PlayerNavMeshLocation,MinDistance,MaxDistance,Enemy); 
    }
    return BestPoint;
}

bool UBTTask_FindNavMeshPoint::IsHittingSomething(const FVector& Start, const FVector& End,const AActor* Enemy)
{
    FHitResult HitResult;
    
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true; 
    TraceParams.bReturnPhysicalMaterial = false; 
    TraceParams.AddIgnoredActor(Enemy);  
            
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult, 
        Start, 
        End,
        ECC_Visibility,
        TraceParams
    );
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 1.0f);  // Linea rossa
    return bHit;
}

bool UBTTask_FindNavMeshPoint::IsInRange(float Distance, const float MinDistance, const float MaxDistance)
{
    return Distance > MinDistance && Distance < MaxDistance;
}

FVector UBTTask_FindNavMeshPoint::GetCorrectNavPoint(TArray<FNavLocation> NavPoints,float BestDistance, const FVector& PlayerLocation, FVector PlayerNavMeshLocation,const float MinDistance, const float MaxDistance,const AActor* Enemy)
{
    FVector BestPoint = FVector::ZeroVector;
    
    for (const FNavLocation& NavPoint : NavPoints)
    {
        float Distance = FVector::Distance(PlayerLocation, NavPoint.Location);
            
        if (IsInRange(Distance,MinDistance,MaxDistance) && !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy) )
        {
            LGDebug::Log("Raycast non colpisce nulla: punto valido", true);
            if (Distance > BestDistance)
            {
                LGDebug::Log("player location : " + FString::SanitizeFloat(PlayerLocation.Z), true);
                LGDebug::Log("Punto considerato valido Z: " + FString::SanitizeFloat(NavPoint.Location.Z), true);
                BestDistance = Distance;
                BestPoint = NavPoint.Location;
            }
        }
    }
    return BestPoint;
}




