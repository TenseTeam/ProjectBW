// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBase/NPCBaseController.h"

#include "AI/NPC/NPCBase/NPCBase.h"


ANPCBaseController::ANPCBaseController()
{
	
}

void ANPCBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPCBase* const EnemyBase = Cast<ANPCBase>(InPawn))
	{
		ControlledPawn = EnemyBase;
		if (UBehaviorTree* const tree = EnemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			InitializeBlackboardValues();
			RunBehaviorTree(tree);
			UE_LOG(LogTemp, Warning, TEXT("Aic controller: %s"), *InPawn->GetName());
		}
	}
}

void ANPCBaseController::InitializeBlackboardValues()
{
	
}

ANPCBase* ANPCBaseController::GetControlledPawn() const
{
	return ControlledPawn;
}

void ANPCBaseController::BeginPlay()
{
	Super::BeginPlay();
}
