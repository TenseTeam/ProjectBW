// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"

bool URPGItemsGenerator::TryGenerateRPGItem(URPGItem* Item, const URPGItemBaseGenerationData* GenerationData)
{
	if (GenerationData == nullptr) return false;

	Item->VisualDetails = GenerateVisualDetails(GenerationData->ItemVisualDetailsSet);

	const URPGInventoriesManager* InventoriesManager = Cast<URPGInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
	Item->RarityLevel = GenerateRarityLevel(InventoriesManager->ItemsRarities);

	return true;
}

bool URPGItemsGenerator::TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData)
{
	if (!TryGenerateRPGItem(GearItem, GenerationData)) return false;
	
	GenerateItemStatsModifiers(GearItem, GenerationData);

	return true;
}

FRPGItemVisualDetails URPGItemsGenerator::GenerateVisualDetails(const URPGItemVisualSetData* Set)
{
	if (!IsValid(Set))
		return FRPGItemVisualDetails();
	
	TArray<FRPGItemVisualDetails> Visuals = Set->Visuals.Array();
	return Visuals[FMath::RandRange(0, Visuals.Num() - 1)];
}

URPGRarityLevelData* URPGItemsGenerator::GenerateRarityLevel(const URPGItemsRaritiesData* Set)
{
	TArray<URPGRarityLevelData*> RarityLevels = Set->RarityLevels.Array();

	const float RandomValue = FMath::FRand();
	float CumulativeProbability = 0.0f;

	for (int32 i = 0; i < RarityLevels.Num(); i++)
	{
		CumulativeProbability += RarityLevels[i]->Probability;
		if (RandomValue <= CumulativeProbability)
			return RarityLevels[i];
	}

	return nullptr;
}

void URPGItemsGenerator::GenerateItemStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData)
{
	TMap<UCoreStatData*, int32> StatsModifiers = TMap<UCoreStatData*, int32>();

	for (const auto& StatModifier : GenerationData->StatsModifiers)
		Item->AddItemStat(StatModifier.Key, StatModifier.Value);
}
