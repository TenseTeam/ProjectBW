// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_rotateWhileInvestigating.h"

#include "AIController.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"

UBTTask_rotateWhileInvestigating::UBTTask_rotateWhileInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Investigate Rotation";
}

EBTNodeResult::Type UBTTask_rotateWhileInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	ANPCBaseEnemy* Pawn = Cast<ANPCBaseEnemy>(AIController->GetPawn());
	ControlledPawn = Pawn;

	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		RotationTimer,
		[this, &OwnerComp, NodeMemory]() { UpdateRotation(OwnerComp, NodeMemory, RotationInterval); },
		RotationInterval,
		true
	);

	return EBTNodeResult::InProgress;
}

void UBTTask_rotateWhileInvestigating::UpdateRotation(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	 if (!ControlledPawn)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }
    
        FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	
        float TargetYaw = CurrentRotation.Yaw + (CurrentDirection * MaxRotationAngle);
	
        float NewYaw = FMath::Lerp(CurrentRotation.Yaw, TargetYaw, RotationSpeed * DeltaTime);
	
        FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
    
        ControlledPawn->SetActorRotation(NewRotation);
        

        if (FMath::Abs(NewYaw - TargetYaw) < 1.0f)  
        {
            CurrentDirection *= -1;
        }
	
}
