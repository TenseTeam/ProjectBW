// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
}

bool AWeaponBase::WeaponAttack()
{
	if (OnWeaponAttack())
	{
		OnWeaponAttackEvent.Broadcast();
		return true;
	}

	return false;
}

bool AWeaponBase::OnWeaponAttack_Implementation()
{
	return true;
}
