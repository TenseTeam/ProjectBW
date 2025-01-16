// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetIdleJumpingLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

// UBTTask_GetIdleJumpingLocation::UBTTask_GetIdleJumpingLocation(FObjectInitializer const& ObjectInitializer)
// {
// 	NodeName = "Get Idle Jumping Location";
// }

// EBTNodeResult::Type UBTTask_GetIdleJumpingLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
// 	if (!BlackboardComp)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
// 	
// 	AAIController* AIController = OwnerComp.GetAIOwner();
// 	if (!AIController)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
//
// 	AActor* ControlledPawn = AIController->GetPawn();
// 	if (!ControlledPawn)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
// 	
// 	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
// 	if (!NavSys)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
//
// 	FVector PlayerJumpLocation = BlackboardComp->GetValueAsVector(PlayerJumpLocationKey.SelectedKeyName);
// 	
// 	FVector JumpStartLocation = GetJumpStartLocation();
// 	
// 	// FNavLocation NavLocation;
// 	// if (NavSys->ProjectPointToNavigation(PlayerJumpLocation, NavLocation, FVector(200.0f)))
// 	// {
// 	// 	
// 	BlackboardComp->SetValueAsVector(InitialJumpingLocationKey.SelectedKeyName,JumpStartLocation);
// 	// 	return EBTNodeResult::Succeeded;
// 	// }
// 	
// 	return EBTNodeResult::Failed;
// }
//
// FVector UBTTask_GetIdleJumpingLocation::GetJumpStartLocation(AActor* ControlledPawn)
// {
// 	FHitResult GroundCheckHit;
// 	FCollisionQueryParams CollisionQueryParams;
// 	CollisionQueryParams.AddIgnoredActor(ControlledPawn);
// 	FVector StartTraceLocation = ControlledPawn->GetActorLocation();
// 	FVector EndTraceLocation = StartTraceLocation + FVector::DownVector * 1000.0f;
// 	GetWorld()->LineTraceSingleByChannel(GroundCheckHit, StartTraceLocation, EndTraceLocation, ECC_Visibility,CollisionQueryParams);
//
// 	float GroundPositionZ = GroundCheckHit.Location.Z;
//
// 	FHitResult HitResult;
// 	StartTraceLocation = FVector(StartTraceLocation.X, StartTraceLocation.Y, GroundPositionZ + 20);
// 	FVector Direction = ControlledPawn->GetActorForwardVector();
// 	EndTraceLocation = StartTraceLocation + Direction * 1000.0f;
// 	GetWorld()->LineTraceSingleByChannel(HitResult,StartTraceLocation,EndTraceLocation,ECC_Visibility,CollisionQueryParams);
//
// 	FVector Normal = HitResult.Normal;
// 	FVector JumpStartLocation = HitResult.Location + Normal * 200.0f;
// 	
// }
