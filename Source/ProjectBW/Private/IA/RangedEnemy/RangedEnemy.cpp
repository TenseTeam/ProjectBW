// Copyright Villains, Inc. All Rights Reserved.


#include "IA/RangedEnemy/RangedEnemy.h"

#include "IA/Weapons/Ranged/RangedWeapon.h"
#include "Utility/LGDebug.h"


// Sets default values
ARangedEnemy::ARangedEnemy()
{
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	ChildActor->SetChildActorClass(ARangedWeapon::StaticClass());
	EnemyType = EEnemyType::Ranged;
}

void ARangedEnemy::OnEnemyDead()
{
	Super::OnEnemyDead();
	EQS_Manager->CanSearchRengedPoint = false;
	//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)EEnemyState::Dead).ToString(),true);
}


void ARangedEnemy::BeginPlay()
{
	RangedWeapon = Cast<ARangedWeapon>(ChildActor->GetChildActor());
	Super::BeginPlay();
	//LGDebug::Log(FString::FromInt(static_cast<int32>(EnemyType)), true);
}

void ARangedEnemy::PostInitProperties()
{
	Super::PostInitProperties();
	ChildActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SwordSocketInHand);
}




