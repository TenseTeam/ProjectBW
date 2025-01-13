// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/BWNPCBaseEnemy/BWNPCBaseEnemy.h"

#include "AI/NPC/BWNPCBaseEnemy/BWNPCbaseEnemyController.h"
#include "Kismet/GameplayStatics.h"



ABWNPCBaseEnemy::ABWNPCBaseEnemy()
{
	AttackTarget = nullptr;
}

FVector ABWNPCBaseEnemy::RandomPosition(FVector Position)
{
	
	const float MinRadius = GetMinInvestigatingRadius();
	const float MaxRadius = GetMaxInvestigatingRadius();
	
	float RandomRadius = FMath::FRandRange(MinRadius, MaxRadius);
	
	float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
	
	float OffsetX = RandomRadius * FMath::Cos(RandomAngle);
	float OffsetY = RandomRadius * FMath::Sin(RandomAngle);
	
	FVector RandomPoint = Position + FVector(OffsetX, OffsetY, 0.0f);

	return RandomPoint;
}

void ABWNPCBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


