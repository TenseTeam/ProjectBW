// Copyright Villains, Inc. All Rights Reserved.


#include "IA/RangedEnemy/RangedEnemy.h"


// Sets default values
ARangedEnemy::ARangedEnemy()
{
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("ShooterComponent"));
	
}

void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARangedEnemy::BaseAttack(AActor* Target)
{
	
}


