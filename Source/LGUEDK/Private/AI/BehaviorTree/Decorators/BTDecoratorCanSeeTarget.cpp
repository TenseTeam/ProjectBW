// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecoratorCanSeeTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecoratorCanSeeTarget::UBTDecoratorCanSeeTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Can See Target";
}

bool UBTDecoratorCanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
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
	
	AActor* TargetActor =Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!TargetActor)
	{
		return false;
	}
	
	FVector StartLocation = ControlledPawn->GetActorLocation();
	FVector EndLocation = TargetActor->GetActorLocation();
	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		TraceChannel
	);
	
	
	if (bDebugLineTrace)
	{
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 1.0f, 0, 2.0f);
	}
	
	return !bHit;
}
