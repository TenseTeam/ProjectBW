// Copyright Villains, Inc. All Rights Reserved.


#include "IA/EnemyBase/EnemyBase.h"

#include "Utility/LGDebug.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	ResourceAttributeManager = CreateDefaultSubobject<UResourceAttributeManager>(TEXT("ResourceAttributeManager"));
	EnemyAttackRequestManage = CreateDefaultSubobject<UEnemyAttackRequestManager>(TEXT("EnemyAttackRequestManager"));
}

int AEnemyBase::GetMyTeamIndex() const
{
	//LGDebug::Log(" MY TEAM: " + EnemyAttackRequestManage->GetTeamIndex(), true);
	return EnemyAttackRequestManage->GetTeamIndex();
	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}





