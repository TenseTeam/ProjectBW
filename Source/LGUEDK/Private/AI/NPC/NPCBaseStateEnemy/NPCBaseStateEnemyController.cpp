// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseStateEnemyController::ANPCBaseStateEnemyController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void ANPCBaseStateEnemyController::SetStateAsPassive()
{
}

void ANPCBaseStateEnemyController::SetStateAsPatrolling()
{
}

void ANPCBaseStateEnemyController::SetStateAsChasing(AActor* Actor)
{
}

void ANPCBaseStateEnemyController::SetStateAsAttacking(AActor* Actor)
{
}

void ANPCBaseStateEnemyController::SetStateAsInvestigating()
{
}

void ANPCBaseStateEnemyController::SetStateAsDead()
{
}

void ANPCBaseStateEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

