// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"

URPGItem* URPGFactory::CreateRPGItem(URPGItemData* Data, const URPGItemBaseGenerationData* GenerationData, const bool bBuildItem, const int32 Quantity)
{
	if (Data == nullptr)
		return nullptr;

	int32 Overflow = 0;
	URPGItem* Item = Cast<URPGItem>(UISFactory::CreateItemInQuantity(Data, Overflow, Quantity));
	Item->Init(Data);

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

	int32 Overflow = 0;
	URPGGearItem* GearItem = Cast<URPGGearItem>(UISFactory::CreateItemInQuantity(Data, Overflow));
	GearItem->Init(Data);

	if (bBuildItem)
	{
		if (!URPGItemsGenerator::TryGenerateRPGGearItem(GearItem, GenerationData))
			UE_LOG(LogInventorySystem, Error, TEXT("Failed to generate RPG Gear Item."));
	}

	return GearItem;
}

UStatOperation* URPGFactory::CreateStatOperationForItem(const TSubclassOf<UStatOperation> OperationClass, URPGItem* Item)
{
	UStatOperation* Operation = NewObject<UStatOperation>(Item, OperationClass);
	Operation->Init(Item, Item->RarityLevel->RarityValue);
	return Operation;
}

UStatOperation* URPGFactory::CreateStatOperationForBridge(const TSubclassOf<UStatOperation> OperationClass, UStatsBridgeBase* Bridge, USpecialStatData* SpecialStatData)
{
	UStatOperation* Operation = NewObject<UStatOperation>(Bridge, OperationClass);

	if (!IsValid(Bridge) || !IsValid(Bridge->SpecialStats))
	{
		UE_LOG(LogStatsSystem, Error, TEXT("Stats Bridge or Special Stats Container not found."));
		return nullptr;
	}
	
	Operation->Init(SpecialStatData, Bridge->SpecialStats->GetStatDefaultValue(SpecialStatData));
	return Operation;
}
