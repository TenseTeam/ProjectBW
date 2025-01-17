// Copyright Villains, Inc. All Rights Reserved.


#include "IA/EnemyBase/EnemyBase.h"



// Sets default values
AEnemyBase::AEnemyBase()
{
	ResourceAttributeManager = CreateDefaultSubobject<UResourceAttributeManager>(TEXT("ResourceAttributeManager"));
}


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}



