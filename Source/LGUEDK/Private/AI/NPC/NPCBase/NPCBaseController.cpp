// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBase/NPCBaseController.h"

#include "AI/NPC/NPCBase/NPCBase.h"
#include "Utility/LGDebug.h"


ANPCBaseController::ANPCBaseController()
{
	ControlledPawn = nullptr;
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
			RunBehaviorTree(tree);
			//LGDebug::Log("aic controller inizializzata",true);
		}
	}
}

void ANPCBaseController::BeginPlay()
{
	Super::BeginPlay();
	InitializeBlackboardValues();
}
