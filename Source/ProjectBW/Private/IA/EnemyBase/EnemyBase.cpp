// Copyright Villains, Inc. All Rights Reserved.


#include "IA/EnemyBase/EnemyBase.h"

#include "IA/EnemyBase/ABaseEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Utility/LGDebug.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	ResourceAttributeManager = CreateDefaultSubobject<UResourceAttributeManager>(TEXT("ResourceAttributeManager"));
	EnemyAttackRequestManage = CreateDefaultSubobject<UEnemyAttackRequestManager>(TEXT("EnemyAttackRequestManager"));
	EnemyAttackRequestManage->SetTeamIndex(0);
}

void AEnemyBase::OnEnemyDead()
{
	OnDeadEnemy.Broadcast();
	ABaseEnemyController* AIController = Cast<ABaseEnemyController>(GetController());
	if (!AIController)return;

	Execute_ReleaseToken(AttackTarget);
	
	AIController->SetStateAsDead();
	AIController->GetBrainComponent()->StopLogic("Dead");
	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	// MyController = Cast<ABaseEnemyController>(GetAIController());
	
}

// void AEnemyBase::OnDeadEvent_Implementation()
// {
// 	OnDead.Broadcast();
// 	ANPCBaseStateEnemyController* AIController = Cast<ANPCBaseStateEnemyController>(GetController());
// 	if (!AIController)return;
// 	AIController->SetStateAsPassive();
// 	AIController->GetBrainComponent()->StopLogic("Dead");
// }







