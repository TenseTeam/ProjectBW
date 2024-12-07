// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/Factories/RPGISFactory.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItemBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGWeaponItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"

URPGItemBase* URPGISFactory::CreateRPGItemBase(URPGItemDataBase* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem)
{
	if (Data == nullptr) return nullptr;
	
	URPGItemBase* Item = Cast<URPGItemBase>(UISFactory::CreateItem(Data));
	Item->Init(Data);

	if (bBuildItem)
	{
		if(!URPGItemsGenerator::TryGenerateRPGItemBase(Item, GenerationData))
			UE_LOG(LogTemp, Error, TEXT("Failed to generate RPG Item Base"));
	}
		
	return Item;
}

URPGWeaponItem* URPGISFactory::CreateRPGWeaponItem(URPGWeaponItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem)
{
	if (Data == nullptr) return nullptr;

	URPGWeaponItem* WeaponItem = Cast<URPGWeaponItem>(UISFactory::CreateItem(Data));
	WeaponItem->Init(Data);

	if (bBuildItem)
	{
		if(!URPGItemsGenerator::TryGenerateRPGWeaponItem(WeaponItem, GenerationData))
			UE_LOG(LogTemp, Error, TEXT("Failed to generate RPG Weapon Item"));
	}
	
	return WeaponItem;
}

URPGGearItem* URPGISFactory::CreateRPGGearItem(URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem)
{
	if (Data == nullptr) return nullptr;

	URPGGearItem* GearItem = Cast<URPGGearItem>(UISFactory::CreateItem(Data));
	GearItem->Init(Data);

	if (bBuildItem)
	{
		if (!URPGItemsGenerator::TryGenerateRPGGearItem(GearItem, GenerationData))
			UE_LOG(LogTemp, Error, TEXT("Failed to generate RPG Gear Item"));
	}

	return GearItem;
}
