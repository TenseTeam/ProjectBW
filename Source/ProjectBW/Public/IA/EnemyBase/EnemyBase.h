// Copyright Villains, Inc. All Rights Reserved.

#pragma once



#include "CoreMinimal.h"
#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Components/TeamComponent/EnemyAttackRequestManager.h"
#include "Features/Gameplay/ResourceAttributeSystem/HealthAttribute.h"
#include "Features/Gameplay/ResourceAttributeSystem/Components/ResourceAttributeManager.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FDeadEnemy);

UCLASS()
class PROJECTBW_API AEnemyBase : public ANPCBaseStateEnemy, public IAITargetInterface
{
	GENERATED_BODY()

public:
	
	AEnemyBase();

	UPROPERTY(BlueprintAssignable)
	FDeadEnemy OnDeadEnemy;

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyDead();

protected:
	UFUNCTION()
	void OnHealthDecreased();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Health")
	UResourceAttributeManager* ResourceAttributeManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Health")
	UHealthAttribute* HealthAttribute;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|AttackRequest")
	UEnemyAttackRequestManager* EnemyAttackRequestManage;


};
