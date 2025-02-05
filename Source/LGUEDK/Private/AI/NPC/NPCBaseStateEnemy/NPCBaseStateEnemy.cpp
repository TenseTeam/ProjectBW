// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"

#include "Kismet/GameplayStatics.h"



ANPCBaseStateEnemy::ANPCBaseStateEnemy()
{
	AttackTarget = nullptr;
}

FVector ANPCBaseStateEnemy::RandomPosition(FVector Position)
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

void ANPCBaseStateEnemy::BeginPlay()
{
	Super::BeginPlay();
	EQS_Manager = Cast<AEQS_Manager>(UGameplayStatics::GetActorOfClass(GetWorld(),AEQS_Manager::StaticClass()));
}


