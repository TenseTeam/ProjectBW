// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_IsWieldingASword.h"

#include "AIController.h"
#include "IA/MeleeEnemy/MeleeEnemy.h"

UBTDecorator_IsWieldingASword::UBTDecorator_IsWieldingASword(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "IsWieldingASword";
}

bool UBTDecorator_IsWieldingASword::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(AIController->GetPawn());
	if (!MeleeEnemy)
	{
		return false;
	}

	if (MeleeEnemy->GetWieldSword())
		return true;
	
	return false;
}
