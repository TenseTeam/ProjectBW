// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "NPCBaseEnemyController.generated.h"

class ANPCBaseEnemy;

UCLASS()
class LGUEDK_API ANPCBaseEnemyController : public ANPCPerceptionSystemController
{
	GENERATED_BODY()

public:
	
	ANPCBaseEnemyController();

protected:
	
	virtual void BeginPlay() override;
	
	virtual void InitializeBlackboardValues() override;
	
	UFUNCTION()
	void SetStateAsPassive();
	
	UFUNCTION()
	void SetStateAsPatrolling();

	UFUNCTION()
	void SetStateAsAttacking(AActor* Actor);

	UFUNCTION()
	void SetStateAsInvestigating();

private:
	
	UPROPERTY()
	ANPCBaseEnemy* MyControlledPawn;
	
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override ;
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) override ;
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) override ;
	
	virtual void OnLostSight() override ;
	virtual void OnLostHear() override ;
	virtual void OnLostDamage() override ;
	
	UFUNCTION()
	FVector RandomPosition(FVector Position);
	
};
