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
		
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPassive() ;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPatrolling() ;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsAttacking(AActor* Actor) ;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsInvestigating() ;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsDead() ;

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
	
};
