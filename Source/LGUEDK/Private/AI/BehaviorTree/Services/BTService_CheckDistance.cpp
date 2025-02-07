// Copyright Villains, Inc. All Rights Reserved.


#include "BTService_CheckDistance.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTService_CheckDistance::UBTService_CheckDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = " Check Player Distance";
	
	ForceInstancing(true);
	
	CachedBlackboardComp = nullptr;
	
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_CheckDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float Distance = CachedBlackboardComp->GetValueAsFloat(DistanceFromPlayerKey.SelectedKeyName);
	float MaxDistance = CachedBlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);

	if (Distance > MaxDistance)
	{
		ANPCBaseStateEnemyController* Controller = Cast<ANPCBaseStateEnemyController>(OwnerComp.GetAIOwner());
		if (Controller)
		{
			AActor* AttackTarget = Cast<AActor>(CachedBlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
			Controller->SetStateAsChasing(AttackTarget);
		}
	}
}
