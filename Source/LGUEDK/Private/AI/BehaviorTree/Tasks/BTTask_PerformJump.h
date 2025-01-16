// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformJump.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_PerformJump : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	explicit UBTTask_PerformJump(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	

private:
	FVector FindSidePoint(const FVector& PlayerPosition, const FVector& EnemyPosition, UNavigationSystemV1* NavSys) const;
	FVector CalculateJumpVelocity(const FVector& Start, const FVector& Target, float Gravity);

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector JumpTargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InitialPositionKey;
	
	/** Debug visualization */
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bEnableDebug = false;
	
	/** Radius of the sphere cast around the player */
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float SphereCastRadius = 200.0f;

	/** Distance to project points to the side of the player */
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float SideOffsetDistance = 10.f;

};

