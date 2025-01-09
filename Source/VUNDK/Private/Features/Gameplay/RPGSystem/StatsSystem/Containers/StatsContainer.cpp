// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/Containers/StatsContainer.h"

class UStatsSaveData;

void UStatsContainer::AddStats(TSet<UStatDataBase*> Stats)
{
	for (UStatDataBase* Stat : Stats)
		AddStat(Stat, Stat->StatMinValue);
}

void UStatsContainer::AddStat(UStatDataBase* Stat, const int32 Value)
{
	if (!Values.Contains(Stat))
		Values.Add(Stat, Stat->StatMinValue);
	
	TrySetValue(Stat, Value);
}

void UStatsContainer::ClearStats()
{
	Values.Empty();
	OnStatsValuesChanged.Broadcast();
}

void UStatsContainer::RemoveStat(UStatDataBase* Stat)
{
	if (!Values.Contains(Stat))
		return;

	Values.Remove(Stat);
	OnStatsValuesChanged.Broadcast();
}

void UStatsContainer::CopyStats(UStatsContainer* StatsContainer)
{
	if (StatsContainer == nullptr)
        return;
	
	Values = StatsContainer->Values;
	OnStatsValuesChanged.Broadcast();
}

bool UStatsContainer::AreStatsEqual(UStatsContainer* StatsContainer) const
{
	if (Values.Num() != StatsContainer->Values.Num())
		return false;

	for (const auto& Pair : Values)
	{
		if (!StatsContainer->Values.Contains(Pair.Key) || StatsContainer->Values[Pair.Key] != Pair.Value)
			return false;
	}

	return true;
}

int32 UStatsContainer::GetValue(const UStatDataBase* Stat) const
{
	if (!Values.Contains(Stat))
		return 0;

	return Values[Stat];
}

bool UStatsContainer::TrySetValue(UStatDataBase* Stat, const int32 Value)
{
	if (!Values.Contains(Stat))
		return false;

	Values[Stat] = Stat->bIsUncapped ? Value : FMath::Clamp(Value, Stat->StatMinValue, Stat->StatMaxValue);
	OnStatsValuesChanged.Broadcast();
	return true;
}

bool UStatsContainer::TryModifyValue(UStatDataBase* Stat, const int32 SumValue)
{
	if (!Values.Contains(Stat))
		return false;

	return TrySetValue(Stat, GetValue(Stat) + SumValue);
}

TMap<UStatDataBase*, int32> const& UStatsContainer::GetValues() const
{
	return Values;
}
