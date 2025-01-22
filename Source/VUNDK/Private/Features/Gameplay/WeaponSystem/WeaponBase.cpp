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
	if (DeployWeaponAttack())
	{
		OnWeaponAttackSuccess();
		OnWeaponAttackSuccessEvent.Broadcast();
		return true;
	}

	OnWeaponAttackFail();
	OnWeaponAttackFailEvent.Broadcast();
	return false;
}

FWeaponData AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

bool AWeaponBase::DeployWeaponAttack_Implementation()
{
	return true;
}

void AWeaponBase::OnWeaponAttackSuccess_Implementation()
{
}

void AWeaponBase::OnWeaponAttackFail_Implementation()
{
}
