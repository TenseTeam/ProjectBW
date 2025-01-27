// Copyright Villains, Inc. All Rights Reserved.


#include "IA/MeleeEnemy/MeleeEnemy.h"

#include "Utility/LGDebug.h"


AMeleeEnemy::AMeleeEnemy()
{
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	ChildActor->SetChildActorClass(AMeleeWeapon::StaticClass());
	EnemyType = EEnemyType::Melee;
}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	MeleeWeapon = Cast<AMeleeWeapon>(ChildActor->GetChildActor());
	
}

void AMeleeEnemy::PostInitProperties()
{
	Super::PostInitProperties();
	ChildActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SwordSocketOnBack);
	
}

void AMeleeEnemy::EquipSword(FName SocketName)
{
	if (ChildActor && GetMesh())
	{
		ChildActor->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			SocketName 
		);
		
		LGDebug::Log("NavLocation: " + ChildActor->GetAttachSocketName().ToString(), true);
	}
}

void AMeleeEnemy::FinishedAnimation()
{
	OnFinishedAnimation.Broadcast();

}

void AMeleeEnemy::OnEnemyDead()
{
	Super::OnEnemyDead();
	EQS_Manager->CanSearchMeleePoint = false;
	//LGDebug::Log("MELEE ENEMY DEAD " + FString::FromInt(static_cast<int32>(CurrentState)), true);
}

