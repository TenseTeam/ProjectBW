// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_CanJumpToElevatedPosition.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"

UBTDecorator_CanJumpToElevatedPosition::UBTDecorator_CanJumpToElevatedPosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Can Jump to Elevated Position";
	
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTDecorator_CanJumpToElevatedPosition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}
	
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}
	
	const FVector SelfPosition = ControlledPawn->GetActorLocation();
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	const FVector TargetPosition = AttackTarget->GetActorLocation();
	
	const float ZDifference = TargetPosition.Z - SelfPosition.Z;
	const float HorizontalDistance = FVector::Dist2D(SelfPosition, TargetPosition);

	UCharacterMovementComponent* MovementComponent = AttackTarget->FindComponentByClass<UCharacterMovementComponent>();

	// LGDebug::Log("SelfZPosition " + FString::SanitizeFloat(SelfPosition), true);
	// LGDebug::Log("TargetZPosition " + FString::SanitizeFloat(TargetZPosition), true);
	
	const float MaxJumpHeight = BlackboardComp->GetValueAsFloat(MaxJumpHeightKey.SelectedKeyName);
	const float AttackRange = BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);
	const float MinJumpDistance = BlackboardComp->GetValueAsFloat(MinJumpDistanceKey.SelectedKeyName);
	
	if (ZDifference > MinJumpDistance && ZDifference <= MaxJumpHeight && HorizontalDistance > AttackRange && !MovementComponent->IsFalling() )
	{
		BlackboardComp->SetValueAsVector(TargetPositionKey.SelectedKeyName, AttackTarget->GetActorLocation());
		return true;
	}
	
	return false;
}
