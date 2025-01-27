// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponMeshRoot);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetArrowColor(FColor::Blue);
#endif
}

void AWeaponBase::Init(APawn* InOwner, UObject* InPayload)
{
	SetPayload(InPayload);
	SetOwner(InOwner);
	SetInstigator(InOwner);
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

void AWeaponBase::SetPayload(UObject* InPayload)
{
	Payload = InPayload;
}

void AWeaponBase::SetWeaponDamage(const float NewDamage)
{
	WeaponData.Damage = NewDamage;
}

float AWeaponBase::GetWeaponDamage() const
{
	return WeaponData.Damage;
}

UObject* AWeaponBase::GetPayload() const
{
	return Payload;
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
