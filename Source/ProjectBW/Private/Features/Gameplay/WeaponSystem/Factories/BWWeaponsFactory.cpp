// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/Factories/BWWeaponsFactory.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"

AWeaponBase* UBWWeaponsFactory::CreateWeapon(APawn* Owner, UBWWeaponItem* WeaponItem)
{
	if (WeaponItem->IsA(UBWWeaponFirearmItem::StaticClass()))
		return CreateWeaponFirearm(Owner, Cast<UBWWeaponFirearmItem>(WeaponItem));

	return CreateWeaponBase(Owner, WeaponItem);
}

AWeaponBase* UBWWeaponsFactory::CreateWeaponBase(APawn* Owner, UBWWeaponItem* WeaponItem)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(SpawnWeaponActor(Owner, WeaponItem));

	if (!IsValid(WeaponBase))
		return nullptr;

	WeaponBase->Init(Owner);
	WeaponBase->SetWeaponDamage(WeaponItem->GetWeaponDamage());
	return WeaponBase;
}

ABWWeaponFirearm* UBWWeaponsFactory::CreateWeaponFirearm(APawn* Owner, UBWWeaponFirearmItem* WeaponItem)
{
	ABWWeaponFirearm* WeaponFirearm = Cast<ABWWeaponFirearm>(SpawnWeaponActor(Owner, WeaponItem));

	if (!IsValid(WeaponFirearm))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::CreateWeaponFirearm: WeaponFirearm is nullptr"));
		return nullptr;
	}

	WeaponFirearm->InitBWWeapon(Owner, WeaponItem);
	return WeaponFirearm;
}

AActor* UBWWeaponsFactory::SpawnWeaponActor(const UObject* Instigator, const UBWWeaponItem* WeaponItem)
{
	if (!IsValid(Instigator))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: Instigator is not valid."));
		return nullptr;
	}

	if (!IsValid(WeaponItem))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponItem is nullptr"));
		return nullptr;
	}

	const UWeaponItemData* WeaponItemData = WeaponItem->GetWeaponItemData();

	if (!IsValid(WeaponItemData))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponItemData is nullptr"));
		return nullptr;
	}

	UWorld* World = Instigator->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: World is nullptr"));
		return nullptr;
	}

	AActor* WeaponActor = World->SpawnActor(WeaponItemData->WeaponClass);

	if (!IsValid(WeaponActor))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponActor is nullptr"));
		return nullptr;
	}

	return WeaponActor;
}
