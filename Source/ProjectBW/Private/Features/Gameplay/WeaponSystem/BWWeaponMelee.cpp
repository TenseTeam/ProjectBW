// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/BWWeaponMelee.h"
#include "Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"

ABWWeaponMelee::ABWWeaponMelee(): WeaponItem(nullptr)
{
}

void ABWWeaponMelee::Init(APawn* InOwner, UObject* InPayload)
{
	Super::Init(InOwner, InPayload);

	if (!IsValid(InPayload))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("ABWWeapon::Init: InPayload is nullptr."));
		return;
	}

	const int32 Damage = GetWeaponMeleeItem()->GetWeaponDamage();
	SetWeaponDamage(Damage);
}

UBWWeaponItem* ABWWeaponMelee::GetWeaponMeleeItem() const
{
	if (!IsValid(WeaponItem))
		return Cast<UBWWeaponItem>(GetPayload());

	return WeaponItem;
}
