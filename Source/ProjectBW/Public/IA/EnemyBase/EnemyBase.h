// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Components/TeamComponent/EnemyAttackRequestManager.h"
#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECTBW_API AEnemyBase : public ANPCBaseStateEnemy, public IAITargetInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();

	///DI DEBUG DA SISTEMARE
	UFUNCTION(BlueprintCallable)
	int GetMyTeamIndex() const;
	
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Health")
	UResourceAttributeManager* ResourceAttributeManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|AttackRequest")
	UEnemyAttackRequestManager* EnemyAttackRequestManage;
	
};
