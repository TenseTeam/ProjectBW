// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"

#include "Components/WidgetComponent.h"

ABWWeaponFirearm::ABWWeaponFirearm(): WeaponFirearmItem(nullptr)
{
	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WeaponWidgetComponent"));
	AmmoWidgetComponent->SetupAttachment(WeaponMesh);
	AmmoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	AmmoWidgetComponent->SetDrawSize(FVector2D(812.0f, 267.0f));
}

void ABWWeaponFirearm::Init(APawn* InOwner, UObject* InPayload)
{
	Super::Init(InOwner, InPayload);
	
	if (!IsValid(InPayload))
	{
		UE_LOG(LogBWWeapons, Display, TEXT("ABWWeaponFirearm::Init: Payload is nullptr."));
		return;
	}
	
	WeaponFirearmItem = GetWeaponFirearmItem();

	const float Damage = WeaponFirearmItem->GetWeaponDamage();
	const float FireRate = WeaponFirearmItem->GetFireRate();
	const float MaxRange = WeaponFirearmItem->GetMaxRange();
	const int32 MagSize = WeaponFirearmItem->GetMagSize();

	WeaponFirearmItem->TryConsumeWeaponItemNewMagazine(MagSize);
	SetWeaponDamage(Damage);
	SetWeaponFireRate(FireRate);
	SetWeaponMaxRange(MaxRange);
	SetWeaponMagSize(MagSize);
	SetCurrentAmmo(WeaponFirearmItem->GetAmmoRemaining());
}

UBWWeaponFirearmItem* ABWWeaponFirearm::GetWeaponFirearmItem() const
{
	if (!IsValid(WeaponFirearmItem))
		return Cast<UBWWeaponFirearmItem>(GetPayload());

	return WeaponFirearmItem;
}

void ABWWeaponFirearm::OnCurrentAmmoChanged_Implementation(int32 CurrentAmmo, int32 MagSize)
{
	Super::OnCurrentAmmoChanged_Implementation(CurrentAmmo, MagSize);
	WeaponFirearmItem->SetAmmoRemaining(Shooter->ShooterBehaviour->GetCurrentAmmo());
}
