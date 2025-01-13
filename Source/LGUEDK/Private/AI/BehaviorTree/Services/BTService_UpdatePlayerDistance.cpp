// Copyright Villains, Inc. All Rights Reserved.


#include "BTService_UpdatePlayerDistance.h"

#include <string>

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTService_UpdatePlayerDistance::UBTService_UpdatePlayerDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = " Update Player Distance";
	
	CachedAttackTarget = nullptr;
	CachedBlackboardComp = nullptr;
	CachedSelf = nullptr;
	
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_UpdatePlayerDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedBlackboardComp)
	{
		return; 
	}

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		LGDebug::Log("Blackboard non trovato", true);
		return;
	}
	
	CachedBlackboardComp = BlackboardComp;
	CachedAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	
	if (CachedAttackTarget == nullptr)
	{
		LGDebug::Log("AttackTarget non trovato", true);
		return;
	}
	CachedSelf = Cast<AActor>(BlackboardComp->GetValueAsObject(SelfKey.SelectedKeyName));
	if (CachedSelf == nullptr)
	{
		LGDebug::Log("Self non trovato", true);
		return;
	}

	CachedBlackboardComp->SetValueAsVector(AttackTargetPositionKey.SelectedKeyName, CachedAttackTarget->GetActorLocation());
	CachedBlackboardComp->SetValueAsVector(PawnPositionKey.SelectedKeyName, CachedSelf->GetActorLocation());
	
	LGDebug::Log("Servizio aggiornamento distanza inizializzato", true);

}

void UBTService_UpdatePlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!CachedAttackTarget)
	{
		LGDebug::Log("Pawn o AttackTarget non trovati", true);
		return;
	}

	if (!CachedBlackboardComp)
	{
		LGDebug::Log("CachedBlackboardComp non trovati", true);
		return;
	}

	CachedBlackboardComp->SetValueAsVector(AttackTargetPositionKey.SelectedKeyName, CachedAttackTarget->GetActorLocation());
	CachedBlackboardComp->SetValueAsVector(PawnPositionKey.SelectedKeyName, CachedSelf->GetActorLocation());
	
	// LGDebug::Log("player position " + CachedAttackTarget->GetActorLocation().ToString(), true);
	// LGDebug::Log("enemy position " + CachedSelf->GetActorLocation().ToString(), true);
}

