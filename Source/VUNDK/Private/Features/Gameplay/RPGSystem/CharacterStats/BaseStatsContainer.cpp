// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/CharacterStats/BaseStatsContainer.h"

void UBaseStatsContainer::Init()
{
	for (UCharacterBaseStatData* BaseStat : BaseStats)
		DefaultValues.Add(BaseStat, BaseStat->StatMinValue);
}

int32 UBaseStatsContainer::GetStatDefaultValue(const UCharacterBaseStatData* BaseStat) const
{
	if (!DefaultValues.Contains(BaseStat))
		return -1;

	return DefaultValues[BaseStat];
}

int32 UBaseStatsContainer::SetStatDefaultValue(UCharacterBaseStatData* BaseStat, const int32 Value)
{
	if (!DefaultValues.Contains(BaseStat))
		return -1;

	DefaultValues[BaseStat] = Value;
	return DefaultValues[BaseStat];
}

int32 UBaseStatsContainer::AddStatDefaultValue(const UCharacterBaseStatData* BaseStat, const int32 Value)
{
	if (!DefaultValues.Contains(BaseStat))
		return -1;

	DefaultValues[BaseStat] += Value;
	return DefaultValues[BaseStat];
}

TMap<UCharacterBaseStatData*, int32> const& UBaseStatsContainer::GetDefaultValues() const
{
	return DefaultValues;
}

UCharacterBaseStatData* UBaseStatsContainer::GetBaseStatByCodeName(const FName& BaseStatCodeName) const
{
	for (UCharacterBaseStatData* BaseStat : BaseStats)
	{
		if (BaseStat->StatCodeName == BaseStatCodeName)
			return BaseStat;
	}

	return nullptr;
}
