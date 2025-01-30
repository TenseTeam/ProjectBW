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
	FStatePassive);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStatePatrolling);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStateInvestigating);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStateChasing , AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStateAttacking , AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStateDead);

UCLASS()
class PROJECTBW_API AEnemyBase : public ANPCBaseStateEnemy, public IAITargetInterface
{
	GENERATED_BODY()

public:
	
	AEnemyBase();
	
	UPROPERTY(BlueprintAssignable)
	FStatePassive OnStatePassive;

	UPROPERTY(BlueprintAssignable)
	FStatePatrolling OnStatePatrolling;

	UPROPERTY(BlueprintAssignable)
	FStateInvestigating OnStateInvestigating;

	UPROPERTY(BlueprintAssignable)
	FStateAttacking OnStateChasing;
	
	UPROPERTY(BlueprintAssignable)
	FStateAttacking OnStateAttacking;

	UPROPERTY(BlueprintAssignable)
	FStateDead OnStateDead;

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyPassive();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyPatrolling();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyInvestigating();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyChasing(AActor* Target);

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyAttack(AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyDead();

	

protected:
	
	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void OnHealthDecreased();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Health")
	UResourceAttributeManager* ResourceAttributeManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Health")
	UHealthAttribute* HealthAttribute;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|AttackRequest")
	UEnemyAttackRequestManager* EnemyAttackRequestManage;


};
