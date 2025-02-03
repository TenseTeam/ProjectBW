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
	bCanAttack = true;
}

void AEnemyBase::OnEnemyPassive()
{
	OnStatePassive.Broadcast();
	SetEnemyState(EEnemyState::Passive);
}

void AEnemyBase::OnEnemyPatrolling()
{
	OnStatePatrolling.Broadcast();
	SetEnemyState(EEnemyState::Patrolling);
	
	if (AttackTarget)
	{
		ReturnAttackToken();
	}
}

void AEnemyBase::OnEnemyInvestigating()
{
	OnStateInvestigating.Broadcast();
	SetEnemyState(EEnemyState::Investigating);
	
	if (AttackTarget)
	{
		ReturnAttackToken();
	}
}

void AEnemyBase::OnEnemyChasing(AActor* Target)
{
	OnStateChasing.Broadcast(Target);
	SetEnemyState(EEnemyState::Chasing);
	SetAttackTarget(Target);
	ReturnAttackToken();
}

void AEnemyBase::OnEnemyAttack(AActor* Target)
{
	OnStateAttacking.Broadcast(Target);
	SetEnemyState(EEnemyState::Attacking);
	SetAttackTarget(Target);
	ReturnAttackToken();
}

void AEnemyBase::OnEnemyDead()
{
	OnStateDead.Broadcast();
	
	ABaseEnemyController* AIController = Cast<ABaseEnemyController>(GetController());
	if (!AIController)return;

	EnemyAttackRequestManage->ReturnAllTokens();
	
	AIController->SetStateAsDead();
	AIController->GetBrainComponent()->StopLogic("Dead");

	USkeletalMeshComponent* MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionProfileName("Ragdoll");
	}

	//LGDebug::Log("DEAD",true);
}

void AEnemyBase::ReturnAttackToken()
{
	int releaseToken = EnemyAttackRequestManage->GetCorrectAttackToken(AttackTarget);
	Execute_ReleaseToken(AttackTarget,releaseToken);
	LGDebug::Log("HO RILASCIATO I TOKEN ",true);
}

void AEnemyBase::OnHealthDecreased_Implementation()
{
	LGDebug::Log("Health Decreased",true);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	ResourceAttributeManager->TryGetAttribute("Health",reinterpret_cast<UResourceAttributeBase*&>(HealthAttribute));
	
	HealthAttribute->OnDecreasedAttribute.AddDynamic(this, &AEnemyBase::OnHealthDecreased);
	HealthAttribute->OnReachedMinValue.AddDynamic(this, &AEnemyBase::OnEnemyDead);
	
}







