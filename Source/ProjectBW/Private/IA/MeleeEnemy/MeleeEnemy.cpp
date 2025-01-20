// Copyright Villains, Inc. All Rights Reserved.


#include "IA/MeleeEnemy/MeleeEnemy.h"



AMeleeEnemy::AMeleeEnemy()
{
	
}

void AMeleeEnemy::EquipSword()
{
	if (EquipSwordMontage && SwordMesh)
	{
		PlayMontageWithNotify(EquipSwordMontage, "EquipSwordNotify", SwordSocketInHand);
	}
}

void AMeleeEnemy::UnequipSword()
{
	if (UnequipSwordMontage && SwordMesh)
	{
		PlayMontageWithNotify(UnequipSwordMontage, "UnequipSwordNotify", SwordSocketOnBack);
	}
}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (bWieldSword)
	{
		EquipSword();
	}
	
}

void AMeleeEnemy::PlayMontageWithNotify(UAnimMontage* MontageToPlay, const FName& NotifyName, const FName& TargetSocket)
{
	
}

void AMeleeEnemy::OnMontageNotifyReceived(const FName& NotifyName, const FName& TargetSocket)
{
	
}
