// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemsGenerator.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"

bool URPGItemsGenerator::TryGenerateRPGItemBase(URPGItemBase* Item, const URPGItemBaseGenerationData* GenerationData)
{
	if (GenerationData == nullptr) return false;

	Item->VisualDetails = GenerateVisualDetails(GenerationData->ItemVisualDetailsSet);

	const URPGInventoriesManager* InventoriesManager = Cast<URPGInventoriesManager>(UISInventoriesUtility::GetInventoriesManager());
	Item->RarityLevel = GenerateRarityLevel(InventoriesManager->ItemsRarities);

	return true;
}

bool URPGItemsGenerator::TryGenerateRPGWeaponItem(URPGWeaponItem* WeaponItem, const URPGItemBaseGenerationData* GenerationData)
{
	if (!TryGenerateRPGItemBase(WeaponItem, GenerationData)) return false;
	
	WeaponItem->WeaponDamageOperation = CreateWeaponDamageOperation(WeaponItem);

	return true;
}

bool URPGItemsGenerator::TryGenerateRPGGearItem(URPGGearItem* GearItem, const URPGGearItemGenerationData* GenerationData)
{
	if (!TryGenerateRPGItemBase(GearItem, GenerationData)) return false;
	
	GearItem->StatsModifiers = GenerateStatsModifiers(GearItem, GenerationData);
	GearItem->MainStat = GetRandomMainStat(GenerationData->MainStatsPool);

	return true;
}

UStatOperation* URPGItemsGenerator::CreateWeaponDamageOperation(URPGWeaponItem* WeaponItem)
{
	UStatOperation* WeaponDamageOperation = NewObject<UStatOperation>(GetTransientPackage(), WeaponItem->GetRPGWeaponData()->WeaponDamageOperation);
	WeaponDamageOperation->Init(WeaponItem, WeaponItem->RarityLevel->RarityValue);
	return WeaponDamageOperation;
}

FRPGItemVisualDetails URPGItemsGenerator::GenerateVisualDetails(const URPGItemVisualSetData* Set)
{
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

TMap<UCharacterBaseStatData*, int32> URPGItemsGenerator::GenerateStatsModifiers(URPGGearItem* Item, const URPGGearItemGenerationData* GenerationData)
{
	TMap<UCharacterBaseStatData*, int32> StatsModifiers = TMap<UCharacterBaseStatData*, int32>();

	for (const auto& StatModifier : GenerationData->StatsModifiers)
	{
		TSubclassOf<UStatOperation> StatOperationClass = StatModifier.Value;
		UStatOperation* Operation = NewObject<UStatOperation>(Item, StatOperationClass);
		Operation->Init(Item, Item->RarityLevel->RarityValue);
		StatsModifiers.Add(StatModifier.Key, Operation->GetResultOperation());
	}

	return StatsModifiers;
}

UCharacterBaseStatData* URPGItemsGenerator::GetRandomMainStat(const URPGMainStatsData* MainStatsPool)
{
	const int32 RandomIndex = FMath::RandRange(0, MainStatsPool->MaintStatPool.Num() - 1);
	return MainStatsPool->MaintStatPool.Array()[RandomIndex];
}
