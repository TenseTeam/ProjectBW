// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
}

void AWeaponBase::Init(APawn* InOwner)
{
	SetOwner(InOwner);
	SetInstigator(InOwner);
}

void AWeaponBase::SetWeaponDamage(const float NewDamage)
{
	WeaponData.Damage = NewDamage;
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

FWeaponData AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

bool AWeaponBase::OnWeaponAttack_Implementation()
{
	return true;
}
