// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseStateEnemyController::ANPCBaseStateEnemyController()
{
	MyControlledPawn = nullptr;
}

void ANPCBaseStateEnemyController::BeginPlay()
{
	MyControlledPawn = Cast<ANPCBaseStateEnemy>(GetControlledPawn());
	Super::BeginPlay();
}