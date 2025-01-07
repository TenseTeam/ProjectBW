// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/BaseStatsContainer.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SaveData/StatsSaveData.h"

class UStatsSaveData;

USaveData* UBaseStatsContainer::CreateSaveData()
{
	UStatsSaveData* SaveData = NewObject<UStatsSaveData>();
	
	for (auto& Pair : BaseValues)
		SaveData->SavedStats.Add(Pair.Key->StatID, Pair.Value);
	
	return SaveData;
}

bool UBaseStatsContainer::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsSaveData* SaveData = Cast<UStatsSaveData>(SavedData);

	for (TMap<FName, int32> BaseStatsMap = SaveData->SavedStats; auto& Pair : BaseStatsMap)
	{
		if (UBaseStatData* BaseStatData = GetBaseStatByID(Pair.Key))
			SetStatBaseValue(BaseStatData, Pair.Value);
	}

	return true;
}

void UBaseStatsContainer::Init()
{
	for (UBaseStatData* BaseStat : BaseStats)
		BaseValues.Add(BaseStat, BaseStat->StatMinValue);
}

int32 UBaseStatsContainer::GetStatBaseValue(const UBaseStatData* BaseStat) const
{
	if (!BaseValues.Contains(BaseStat))
		return -1;

	return BaseValues[BaseStat];
}

int32 UBaseStatsContainer::SetStatBaseValue(UBaseStatData* BaseStat, const int32 Value)
{
	if (!BaseValues.Contains(BaseStat))
		return -1;
	
	BaseValues[BaseStat] = BaseStat->bIsUncapped ? Value : FMath::Clamp(Value, BaseStat->StatMinValue, BaseStat->StatMaxValue);
	return BaseValues[BaseStat];
}

int32 UBaseStatsContainer::ModifyStatBaseValue(UBaseStatData* BaseStat, const int32 SumValue)
{
	if (!BaseValues.Contains(BaseStat))
		return -1;
	
	return SetStatBaseValue(BaseStat, GetStatBaseValue(BaseStat) + SumValue);
}

UBaseStatData* UBaseStatsContainer::GetBaseStatByCodeName(const FName& BaseStatCodeName) const
{
	for (UBaseStatData* BaseStat : BaseStats)
	{
		if (BaseStat->StatCodeName.IsEqual(BaseStatCodeName))
			return BaseStat;
	}

	return nullptr;
}

UBaseStatData* UBaseStatsContainer::GetBaseStatByID(const FName& BaseStatID) const
{
	for (UBaseStatData* BaseStat : BaseStats)
	{
		if (BaseStat->StatID.IsEqual(BaseStatID))
			return BaseStat;
	}

	return nullptr;
}

TMap<UBaseStatData*, int32> const& UBaseStatsContainer::GetBaseValues() const
{
	return BaseValues;
}
