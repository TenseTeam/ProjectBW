// Copyright Villains, Inc. All Rights Reserved.


#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"

#include "AI/NPC/NPCBase/NPCBaseController.h"


ANPCBase::ANPCBase()
{
	
}

UBehaviorTree* ANPCBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
}
