// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_PerformJump.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_PerformJump::UBTTask_PerformJump(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Perform Jump";
}

EBTNodeResult::Type UBTTask_PerformJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

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
    
    const FVector PlayerPosition = BlackboardComp->GetValueAsVector(JumpTargetLocationKey.SelectedKeyName);
    const FVector EnemyPosition = ControlledPawn->GetActorLocation();

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    // FVector TraceStart = PlayerPosition;
    // FVector TraceEnd = PlayerPosition;
    // FHitResult HitResult;
    //
    // bool bHit = UKismetSystemLibrary::SphereTraceSingle(
    //     AIController->GetWorld(),
    //     TraceStart,
    //     TraceEnd,
    //     SphereCastRadius,
    //     UEngineTypes::ConvertToTraceType(ECC_Visibility),
    //     false,
    //     TArray<AActor*>(),
    //     bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
    //     HitResult,
    //     true
    // );
    //
    // if (bHit)
    // {
    //     FVector SidePoint = FindSidePoint(PlayerPosition, EnemyPosition, NavSys);
    //     if (SidePoint.IsZero())
    //     {
    //         LGDebug::Log("No valid side point found", true);
    //         return EBTNodeResult::Failed;
    //     }
    //
    //     FVector JumpDirection = (SidePoint - EnemyPosition).GetSafeNormal();
    //     JumpDirection.Z = 1.5f;  // Aumenta l'inclinazione del salto per una traiettoria più alta
    //     JumpDirection.Normalize();
    //
    //     // Modifica il calcolo della velocità per tener conto della distanza orizzontale e verticale
    //     FVector JumpVelocity = CalculateJumpVelocity(EnemyPosition, SidePoint, 1000.0f);  // Prova una gravità maggiore
    //
    //     if (JumpVelocity.IsZero())
    //     {
    //         if (bEnableDebug)
    //         {
    //             UE_LOG(LogTemp, Warning, TEXT("Failed to calculate valid jump velocity."));
    //         }
    //         return EBTNodeResult::Failed;
    //     }
    //
    //     if (ACharacter* Character = Cast<ACharacter>(ControlledPawn))
    //     {
    //         if (Character->CanJump())
    //         {
    //             Character->LaunchCharacter(JumpVelocity, true, true);
    //
    //             if (bEnableDebug)
    //             {
    //                 UE_LOG(LogTemp, Warning, TEXT("Jump executed: Velocity: %s"), *JumpVelocity.ToString());
    //             }
    //
    //             FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    //             return EBTNodeResult::Succeeded;
    //         }
    //     }
    // }
    FVector JumpDirection = (PlayerPosition - EnemyPosition).GetSafeNormal();
    JumpDirection.Z = 1.5f;  // Aumenta l'inclinazione del salto per una traiettoria più alta
    JumpDirection.Normalize();
    
    // Modifica il calcolo della velocità per tener conto della distanza orizzontale e verticale
    FVector JumpVelocity = CalculateJumpVelocity(EnemyPosition, PlayerPosition, 1000);
    
    if (JumpVelocity.IsZero())
    {
        if (bEnableDebug)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to calculate valid jump velocity."));
        }
        return EBTNodeResult::Failed;
    }
    
    if (ACharacter* Character = Cast<ACharacter>(ControlledPawn))
    {
        if (Character->CanJump())
        {
            Character->LaunchCharacter(JumpVelocity, true, true);
    
            if (bEnableDebug)
            {
                UE_LOG(LogTemp, Warning, TEXT("Jump executed: Velocity: %s"), *JumpVelocity.ToString());
            }
    
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }
    }
    
    return EBTNodeResult::Failed;
}

FVector UBTTask_PerformJump::FindSidePoint(const FVector& PlayerPosition, const FVector& EnemyPosition, UNavigationSystemV1* NavSys) const
{
    // FVector ClosestSidePoint = FVector::ZeroVector;
    //
    // FVector ForwardVector = (EnemyPosition - PlayerPosition).GetSafeNormal();
    // FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector);
    //
    // // Try left and right of the player
    // TArray<FVector> SidePoints = {
    //     PlayerPosition + (RightVector * SideOffsetDistance),
    //     PlayerPosition - (RightVector * SideOffsetDistance)
    // };
    //
    // for (const FVector& SidePoint : SidePoints)
    // {
    //     FNavLocation NavLocation;
    //     if (NavSys->ProjectPointToNavigation(SidePoint, NavLocation, FVector(SphereCastRadius)))
    //     {
    //         if (bEnableDebug)
    //         {
    //             DrawDebugSphere(GWorld, SidePoint, 50.0f, 12, FColor::Blue, false, 2.0f);
    //         }
    //
    //         return NavLocation.Location;
    //     }
    // }
    //
    //return ClosestSidePoint;
    return FVector::ZeroVector;
}

FVector UBTTask_PerformJump::CalculateJumpVelocity(const FVector& Start, const FVector& Target, float Gravity)
{
    FVector Displacement = Target - Start;
    
    float ZDifference = Displacement.Z;
    Displacement.Z = 0.0f;
    // // float HorizontalDistance = Displacement.Size();
    //
    // if (HorizontalDistance <= 0.0f)
    // {
    //     return FVector::ZeroVector;
    // }
    
    //float Time = FMath::Sqrt(2 * ZDifference / Gravity);
    
    // float VerticalSpeed = Gravity * Time;
    
    // float HorizontalSpeed = HorizontalDistance / Time;
    
    // FVector HorizontalDirection = Displacement.GetSafeNormal();
    // FVector Velocity = HorizontalDirection * HorizontalSpeed;
    // Velocity.Z = VerticalSpeed;

    // return Velocity;

    float HorizontalDistance = FVector(Displacement.X, Displacement.Y, 0.0f).Size();
    float VerticalSpeed = FMath::Sqrt(2 * Gravity * ZDifference) + 100;
    
    
    return FVector(Displacement.X , Displacement.Y , VerticalSpeed);
}