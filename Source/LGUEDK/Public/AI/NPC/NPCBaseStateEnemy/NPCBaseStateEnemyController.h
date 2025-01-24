// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "NPCBaseStateEnemyController.generated.h"

class ANPCBaseStateEnemy;

UCLASS()
class LGUEDK_API ANPCBaseStateEnemyController : public ANPCPerceptionSystemController
{
	GENERATED_BODY()

public:
	
	ANPCBaseStateEnemyController();
		
	UFUNCTION(BlueprintCallable)
	virtual void SetStateAsPassive() {};
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStateAsPatrolling() {};

	UFUNCTION(BlueprintCallable)
	virtual void SetStateAsAttacking(AActor* Actor) {};

	UFUNCTION(BlueprintCallable)
	virtual void SetStateAsInvestigating() {};

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override { Super::OnPossess(InPawn); };
	
	virtual void InitializeBlackboardValues() override { Super::InitializeBlackboardValues(); };
	
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleSight(Actor, Stimulus); };
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleHear(Actor, Stimulus); };
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleDamage(Actor, Stimulus); };
	
	virtual void OnLostSight() override { Super::OnLostSight(); };
	virtual void OnLostHear() override { Super::OnLostHear(); };
	virtual void OnLostDamage() override { Super::OnLostDamage(); };
	
private:
	
	UPROPERTY()
	ANPCBaseStateEnemy* MyControlledPawn;
	
	
};
