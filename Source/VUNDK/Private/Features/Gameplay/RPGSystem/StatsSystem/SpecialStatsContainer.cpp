// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/SpecialStatsContainer.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsSaveData.h"

USaveData* USpecialStatsContainer::CreateSaveData()
{
	UStatsSaveData* SaveData = NewObject<UStatsSaveData>();

	for (auto& Pair : DefaultValues)
		SaveData->SavedStats.Add(Pair.Key->StatID, Pair.Value);

	return SaveData;
}

bool USpecialStatsContainer::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsSaveData* SaveData = Cast<UStatsSaveData>(SavedData);

	for (TMap<FName, int32> SpecialStatsMap = SaveData->SavedStats; auto& Pair : SpecialStatsMap)
	{
		if (USpecialStatData* SpecialStatData = GetSpecialStatByID(Pair.Key))
			SetStatDefaultValue(SpecialStatData, Pair.Value);
	}

	return true;
}

void USpecialStatsContainer::Init()
{
	for (USpecialStatData* SpecialStat : SpecialStats)
		DefaultValues.Add(SpecialStat, SpecialStat->StatDefaultValue);
}

int32 USpecialStatsContainer::GetStatDefaultValue(const USpecialStatData* SpecialStat) const
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	return DefaultValues[SpecialStat];
}

int32 USpecialStatsContainer::SetStatDefaultValue(USpecialStatData* SpecialStat, const int32 Value)
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	DefaultValues[SpecialStat] = Value;
	OnSpecialStatDefaultValueChanged.Broadcast();
	return DefaultValues[SpecialStat];
}

int32 USpecialStatsContainer::ModifyStatDefaultValue(USpecialStatData* SpecialStat, const int32 SumValue)
{
	if (!DefaultValues.Contains(SpecialStat))
		return -1;

	return SetStatDefaultValue(SpecialStat, GetStatDefaultValue(SpecialStat) + SumValue);
}

TMap<USpecialStatData*, int32> const& USpecialStatsContainer::GetDefaultValues() const
{
	return DefaultValues;
}

USpecialStatData* USpecialStatsContainer::GetSpecialStatByCodeName(const FName& SpecialStatCodeName) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatCodeName.IsEqual(SpecialStatCodeName))
			return SpecialStat;
	}

	return nullptr;
}

USpecialStatData* USpecialStatsContainer::GetSpecialStatByID(const FName& SpecialStatID) const
{
	for (USpecialStatData* SpecialStat : SpecialStats)
	{
		if (SpecialStat->StatID.IsEqual(SpecialStatID))
			return SpecialStat;
	}

	return nullptr;
}
