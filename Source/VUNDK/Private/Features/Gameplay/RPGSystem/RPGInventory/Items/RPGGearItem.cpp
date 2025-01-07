// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"

URPGGearItem::URPGGearItem()
{
}

FRPGGearItemSaveData URPGGearItem::CreateRPGGearItemSaveData() const
{
	FRPGGearItemSaveData GearSaveData;
	GearSaveData.RPGItemSaveData = CreateRPGItemSaveData();

	for (const auto& StatModifier : GearStats)
		GearSaveData.GearBonusStats.Add(StatModifier.Key->StatID, StatModifier.Value);

	return GearSaveData;
}

void URPGGearItem::LoadRPGGearItemSaveData(FRPGGearItemSaveData& GearSaveData, URPGInventory* Inventory)
{
	FRPGItemSaveData ItemSaveData = GearSaveData.RPGItemSaveData;
	LoadRPGItemSaveData(ItemSaveData, Inventory);

	for (TMap<FName, int32> BonusStats = GearSaveData.GearBonusStats; const auto& Stats : BonusStats)
	{
		if (UBaseStatData* StatData = URPGInventoriesUtility::GetStatByID(Stats.Key); IsValid(StatData))
			GearStats.Add(StatData, Stats.Value);
	}
}

int32 URPGGearItem::GetItemStatValue(const UBaseStatData* Stat) const
{
	if (!GearStats.Contains(Stat))
		return 0;

	return GearStats[Stat];
}

void URPGGearItem::AddItemStat(UBaseStatData* Stat, const TSubclassOf<UItemStatOperation> OperationClass, const bool bOverrideIfExist)
{
	auto CalculateResult  = [this](const UBaseStatData* LocalStat, const TSubclassOf<UItemStatOperation>& LocalOperationClass) -> int32
	{
		const UStatOperation* Operation = URPGFactory::CreateItemStatOperation(LocalOperationClass, this);
		return LocalStat->bIsUncapped ? Operation->GetResultOperation() : FMath::Clamp(Operation->GetResultOperation(), LocalStat->StatMinValue, LocalStat->StatMaxValue);
	};
	
	if (GearStats.Contains(Stat))
	{
		if (!bOverrideIfExist)
			return;

		if (const int32 Value = CalculateResult (Stat, OperationClass); Value != 0)
			GearStats[Stat] = Value;
		else
			RemoveItemStat(Stat);
		
		return;
	}

	if (const int32 Value = CalculateResult (Stat, OperationClass); Value != 0)
		GearStats.Add(Stat, CalculateResult (Stat, OperationClass));
	else
		RemoveItemStat(Stat);
}

void URPGGearItem::RemoveItemStat(UBaseStatData* Stat)
{
	if (!GearStats.Contains(Stat))
		return;

	GearStats.Remove(Stat);
}

void URPGGearItem::SetItemStat(UBaseStatData* Stat, const int32 Value)
{
	if (!GearStats.Contains(Stat))
		return;
	
	GearStats[Stat] = Value;
}

void URPGGearItem::ModifyItemStat(UBaseStatData* Stat, const int32 SumValue)
{
	if (!GearStats.Contains(Stat))
		return;

	SetItemStat(Stat, GearStats[Stat] + SumValue);
}

URPGGearItemData* URPGGearItem::GetRPGGearItemData() const
{
	return Cast<URPGGearItemData>(ItemData);
}

bool URPGGearItem::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	if (!Super::CanStackItem_Implementation(OtherItem))
		return false;

	const URPGGearItem* GearItem = Cast<URPGGearItem>(OtherItem);

	if (GearItem == nullptr)
		return false;

	auto AreStatsEqual = [](const TMap<UBaseStatData*, int32>& Map1, const TMap<UBaseStatData*, int32>& Map2) -> bool
	{
		if (Map1.Num() != Map2.Num())
			return false;

		for (const auto& Elem : Map1)
		{
			const int32* Value = Map2.Find(Elem.Key);
			if (Value == nullptr || *Value != Elem.Value)
				return false;
		}

		return true;
	};

	return AreStatsEqual(GearStats, GearItem->GearStats);
}
