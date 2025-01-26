// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "ABaseEnemyController.generated.h"

class ANPCBaseStateEnemy;

UCLASS()
class PROJECTBW_API ABaseEnemyController : public ANPCBaseStateEnemyController
{
	GENERATED_BODY()

public:

	ABaseEnemyController();

	virtual void SetStateAsPassive() override;

	virtual void SetStateAsPatrolling() override;

	virtual void SetStateAsAttacking(AActor* Actor) override;
	
	virtual void SetStateAsInvestigating() override;
	
protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override ;
	
	virtual void InitializeBlackboardValues() override;
	
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override ;
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) override ;
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) override ;
	
	virtual void OnLostSight() override ;
	virtual void OnLostHear() override ;
	virtual void OnLostDamage() override ;
	
private:
	
	UPROPERTY()
	AEnemyBase* EnemyBase;

	UFUNCTION()
	int GetTeamIndex (AActor* Actor);

};
