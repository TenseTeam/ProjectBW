// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseEnemyController::ANPCBaseEnemyController()
{
	MyControlledPawn = nullptr;
}

void ANPCBaseEnemyController::BeginPlay()
{
	MyControlledPawn = Cast<ANPCBaseEnemy>(GetControlledPawn());
	Super::BeginPlay();
}