// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"

URPGItem* URPGFactory::CreateRPGGenericItem(URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem, const int32 Quantity)
{
	if (Data == nullptr)
		return nullptr;

	URPGItem* Item = Cast<URPGItem>(UISFactory::CreateItem(Data));

	if (bBuildItem)
	{
		if (!URPGItemsGenerator::TryGenerateRPGItem(Item, GenerationData))
			UE_LOG(LogInventorySystem, Error, TEXT("Failed to generate RPG Item Base."));
	}

	return Item;
}

URPGGearItem* URPGFactory::CreateRPGGearItem(URPGGearItemData* Data, const URPGGearItemGenerationData* GenerationData, const bool bBuildItem)
{
	if (Data == nullptr)
		return nullptr;

	URPGGearItem* GearItem = Cast<URPGGearItem>(UISFactory::CreateItem(Data));

	if (bBuildItem)
	{
		if (!URPGItemsGenerator::TryGenerateRPGGearItem(GearItem, GenerationData))
			UE_LOG(LogInventorySystem, Error, TEXT("Failed to generate RPG Gear Item."));
	}

	return GearItem;
}

URPGItemStatOperation* URPGFactory::CreateItemStatOperation(const TSubclassOf<URPGItemStatOperation> OperationClass, URPGItem* Item)
{
	URPGItemStatOperation* Operation = NewObject<URPGItemStatOperation>(Item, OperationClass);
	Operation->Init(Item, Item->RarityLevel->RarityValue);
	return Operation;
}

UStatOperation* URPGFactory::CreateBridgeStatOperation(UStatsBridgeBase* Bridge, USpecialStatData* SpecialStatData, const TSubclassOf<UStatOperation>& OperationClass)
{
	UStatOperation* Operation = NewObject<UStatOperation>(Bridge, OperationClass);

	if (!IsValid(Bridge) || !IsValid(Bridge->SpecialStatsContainer))
	{
		UE_LOG(LogStatsSystem, Error, TEXT("Stats Bridge or Special Stats Container not found."));
		return nullptr;
	}
	
	Operation->Init(SpecialStatData, Bridge->SpecialStatsContainer->GetValueAsFloat(SpecialStatData));
	return Operation;
}
