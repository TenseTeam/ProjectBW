// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/Enumerators/EnemyState.h"
#include "AI/NPC/NPCBase/NPCBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NPCBaseEnemy.generated.h"

UCLASS()
class LGUEDK_API ANPCBaseEnemy : public ANPCBase
{
	GENERATED_BODY()

public:
	
	ANPCBaseEnemy();

	void SetMovementSpeed(EMovementSpeed MovementSpeed) const;

	EEnemyState GetState() const { return CurrentState; }
	void SetEnemyState(EEnemyState NewState) { CurrentState = NewState; }
	
	int GetMinInvestigatingRadius() const { return MinInvestigatingRadius; }
	int GetMaxInvestigatingRadius() const { return MaxInvestigatingRadius; }
	float GetTimeBeforeInvestigating() const { return TimeBeforeInvestigating; }
	float GetRandomInvestigatingTimeDeviation() const { return RandomInvestigatingTimeDeviation; }

	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float IdleSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	EEnemyState CurrentState = EEnemyState::Patrolling;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
	int MinInvestigatingRadius = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	int MaxInvestigatingRadius = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float TimeBeforeInvestigating = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	float RandomInvestigatingTimeDeviation = 2.f;

	virtual void BeginPlay() override;

};
