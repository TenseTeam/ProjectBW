// Copyright Villains, Inc. All Rights Reserved.


#include "IA/RangedEnemy/RangedEnemy.h"

#include "Utility/LGDebug.h"


// Sets default values
ARangedEnemy::ARangedEnemy()
{
	//ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	//ChildActor->SetChildActorClass(AMeleeWeapon::StaticClass());
	EnemyType = EEnemyType::Ranged;
}

void ARangedEnemy::OnEnemyDead_Implementation()
{
	Super::OnEnemyDead_Implementation();
	EQS_Manager->CanSearchRengedPoint = false;
	//LGDebug::Log("MORTO RANGED ENEMY " + FString::FromInt(static_cast<int32>(CurrentState)), true);
}


void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//LGDebug::Log(FString::FromInt(static_cast<int32>(EnemyType)), true);
}

void ARangedEnemy::PostInitProperties()
{
	Super::PostInitProperties();
}




