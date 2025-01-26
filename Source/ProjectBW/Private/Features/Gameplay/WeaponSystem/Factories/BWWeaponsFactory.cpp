// Copyright Villains, Inc. All Rights Reserved.

#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/Factories/BWWeaponsFactory.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"
#include "Features/Gameplay/WeaponSystem/BWWeaponMelee.h"
#include "ProjectBW/Public/Features/Gameplay/WeaponSystem/BWWeaponFirearm.h"

AWeaponBase* UBWWeaponsFactory::CreateWeapon(APawn* Owner, UBWWeaponItem* WeaponItem)
{
	if (WeaponItem->IsA(UBWWeaponFirearmItem::StaticClass()))
		return CreateBWWeaponFirearm(Owner, Cast<UBWWeaponFirearmItem>(WeaponItem));

	return CreateBWWeaponMelee(Owner, WeaponItem);
}

ABWWeaponMelee* UBWWeaponsFactory::CreateBWWeaponMelee(APawn* Owner, UBWWeaponItem* WeaponItem)
{
	ABWWeaponMelee* Weapon = Cast<ABWWeaponMelee>(SpawnWeaponActor(Owner, WeaponItem));

	if (!IsValid(Weapon))
		return nullptr;

	Weapon->Init(Owner, WeaponItem);
	return Weapon;
}

ABWWeaponFirearm* UBWWeaponsFactory::CreateBWWeaponFirearm(APawn* Owner, UBWWeaponFirearmItem* WeaponItem)
{
	ABWWeaponFirearm* WeaponFirearm = Cast<ABWWeaponFirearm>(SpawnWeaponActor(Owner, WeaponItem));

	if (!IsValid(WeaponFirearm))
	{
		UE_LOG(LogBWWeapons, Error, TEXT("UBWWeaponsFactory::CreateWeaponFirearm: WeaponFirearm is nullptr"));
		return nullptr;
	}

	WeaponFirearm->Init(Owner, WeaponItem);
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
