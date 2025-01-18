// Copyright Villains, Inc. All Rights Reserved.


#include "IA/RangedEnemy/RangedEnemy.h"


// Sets default values
ARangedEnemy::ARangedEnemy()
{
	Shooter = CreateDefaultSubobject<UShooter>(TEXT("ShooterComponent"));
	
}

void ARangedEnemy::BaseAttack_Implementation()
{
	
}

void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}



