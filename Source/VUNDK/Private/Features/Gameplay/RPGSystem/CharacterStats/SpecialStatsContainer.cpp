// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/CharacterStats/SpecialStatsContainer.h"

void USpecialStatsContainer::Init()
{
	for (UCharacterSpecialStatData* SpecialStat : SpecialStats)
		DefaultValues.Add(SpecialStat, SpecialStat->StatDefaultValue);
}

int32 USpecialStatsContainer::GetStatDefaultValue(const UCharacterSpecialStatData* SpecialStat) const
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	return DefaultValues[SpecialStat];
}

int32 USpecialStatsContainer::SetStatDefaultValue(UCharacterSpecialStatData* SpecialStat, const int32 Value)
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	DefaultValues[SpecialStat] = Value;
	OnSpecialStatDefaultValueChanged.Broadcast();
	return DefaultValues[SpecialStat];
}

int32 USpecialStatsContainer::AddStatDefaultValue(UCharacterSpecialStatData* SpecialStat, const int32 Value)
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	DefaultValues[SpecialStat] += Value;
	OnSpecialStatDefaultValueChanged.Broadcast();
	return DefaultValues[SpecialStat];
}

TMap<UCharacterSpecialStatData*, int32> const& USpecialStatsContainer::GetDefaultValues() const
{
	return DefaultValues;
}

UCharacterSpecialStatData* USpecialStatsContainer::GetSpecialStatByCodeName(const FName& SpecialStatCodeName) const
{
	for (UCharacterSpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatCodeName == SpecialStatCodeName)
			return SpecialStat;
	}

	return nullptr;
}
