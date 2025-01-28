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
	
	AIController->SetStateAsDead();
	AIController->GetBrainComponent()->StopLogic("Dead");

	USkeletalMeshComponent* MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionProfileName("Ragdoll");
	}

	LGDebug::Log("DEAD",true);
}

void AEnemyBase::OnHealthDecreased()
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







