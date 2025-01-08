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

	virtual void OnPossess(APawn* InPawn) override { Super::OnPossess(InPawn); };
	
	virtual void InitializeBlackboardValues() override { Super::InitializeBlackboardValues(); };
	
	UFUNCTION()
	virtual void SetStateAsPassive() {};
	
	UFUNCTION()
	virtual void SetStateAsPatrolling() {};

	UFUNCTION()
	virtual void SetStateAsAttacking(AActor* Actor) {};

	UFUNCTION()
	virtual void SetStateAsInvestigating() {};
	
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleSight(Actor, Stimulus); };
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleHear(Actor, Stimulus); };
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleDamage(Actor, Stimulus); };
	
	virtual void OnLostSight() override { Super::OnLostSight(); };
	virtual void OnLostHear() override { Super::OnLostHear(); };
	virtual void OnLostDamage() override { Super::OnLostDamage(); };
	
private:
	
	UPROPERTY()
	ANPCBaseEnemy* MyControlledPawn;
	
	
};
