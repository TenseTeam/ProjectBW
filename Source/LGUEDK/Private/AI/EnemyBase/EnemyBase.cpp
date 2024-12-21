// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EnemyBase/EnemyBase.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	AIControllerClass = AEnemyControllerBase::StaticClass();
}

UBehaviorTree* AEnemyBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}



