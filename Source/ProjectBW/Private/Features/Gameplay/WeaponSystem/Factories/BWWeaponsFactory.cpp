// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/WeponSystem/Factories/BWWeaponsFactory.h"
#include "Features/Gameplay/InventorySystem/Data/WeaponItemData.h"

AWeaponBase* UBWWeaponsFactory::CreateWeapon(UObject* Instigator, UWeaponItem* WeaponItem)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(SpawnWeaponActor(Instigator, WeaponItem));

	if (!IsValid(WeaponBase))
		return nullptr;
	
	WeaponBase->SetWeaponDamage(WeaponItem->GetWeaponDamage());
	return WeaponBase;
}

AWeaponFirearm* UBWWeaponsFactory::CreateWeaponFirearm(UObject* Instigator, UWeaponFirearmItem* WeaponItem)
{
	AWeaponFirearm* WeaponFirearm = Cast<AWeaponFirearm>(SpawnWeaponActor(Instigator, WeaponItem));

	if (!IsValid(WeaponFirearm))
		return nullptr;
	
	WeaponFirearm->SetWeaponDamage(WeaponItem->GetWeaponDamage());
	WeaponFirearm->SetWeaponFireRate(WeaponItem->GetWeaponFireRate());
	WeaponFirearm->SetWeaponRange(WeaponItem->GetWeaponRange());
	WeaponFirearm->SetWeaponMagSize(WeaponItem->GetWeaponMagSize());
	return WeaponFirearm;
}

AActor* UBWWeaponsFactory::SpawnWeaponActor(const UObject* Instigator, const UWeaponItem* WeaponItem)
{
	if (!IsValid(Instigator))
	{
		UE_LOG(LogTemp, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: Instigator is not valid."));
		return nullptr;
	}
	
	if (!IsValid(WeaponItem))
	{
		UE_LOG(LogTemp, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponItem is nullptr"));
		return nullptr;
	}
	
	const UWeaponItemData* WeaponItemData = WeaponItem->GetWeaponItemData();
	
	if (!IsValid(WeaponItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponItemData is nullptr"));
		return nullptr;
	}

	UWorld* World = Instigator->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: World is nullptr"));
		return nullptr;
	}
	
	AActor* WeaponActor = World->SpawnActor(WeaponItemData->WeaponClass);

	if (!IsValid(WeaponActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UBWWeaponsFactory::SpawnWeaponActor: WeaponActor is nullptr"));
		return nullptr;
	}

	return WeaponActor;
}
