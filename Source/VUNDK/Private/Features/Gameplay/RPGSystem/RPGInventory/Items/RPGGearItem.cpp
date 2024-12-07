// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Utility/CharacterStatsUtility.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"

URPGGearItem::URPGGearItem()
{
	StatsModifiers = TMap<UCharacterBaseStatData*, int32>();
}

FRPGGearItemSaveData URPGGearItem::CreateRPGGearItemSaveData() const
{
	FRPGGearItemSaveData GearSaveData;
	GearSaveData.RPGItemSaveData = CreateRPGItemSaveData();
	GearSaveData.MainStatCodeName = MainStat->StatCodeName;

	for (const auto& StatModifier : StatsModifiers)
		GearSaveData.GearBonusStats.Add(StatModifier.Key->StatCodeName, StatModifier.Value);
	
	return GearSaveData;
}

void URPGGearItem::LoadRPGGearItemSaveData(FRPGGearItemSaveData& GearSaveData, URPGInventory* Inventory)
{
	FRPGItemSaveData ItemSaveData = GearSaveData.RPGItemSaveData;
	LoadRPGItemSaveData(ItemSaveData, Inventory);
	
	const UBaseStatsContainer* BaseStats = UCharacterStatsUtility::GetCharacterStats()->BaseStats;
	MainStat = BaseStats->GetBaseStatByCodeName(GearSaveData.MainStatCodeName);
	
	TMap<FName, int32> BonusStats = GearSaveData.GearBonusStats;
	for (const auto& Stats : BonusStats)
	{
		if (UCharacterBaseStatData* StatData = BaseStats->GetBaseStatByCodeName(Stats.Key))
			StatsModifiers.Add(StatData, Stats.Value);
	}
}

URPGGearItemData* URPGGearItem::GetRPGGearItemData() const
{
	return Cast<URPGGearItemData>(ItemData);
}
