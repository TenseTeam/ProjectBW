// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/Containers/CoreStatsContainer.h"

void UCoreStatsContainer::AddCoreStats(TSet<UCoreStatData*> CoreStats)
{
	for (UCoreStatData* CoreStat : CoreStats)
		AddStat(CoreStat, CoreStat->StatMinValue);
}

TMap<UCoreStatData*, int32> UCoreStatsContainer::GetCoreStatsValues()
{
	TMap<UCoreStatData*, int32> CoreStatsValues;
	
	for (auto& Stat : Values)
	{
		if (UCoreStatData* CoreStat = Cast<UCoreStatData>(Stat.Key))
			CoreStatsValues.Add(CoreStat, Stat.Value);
	}
	
	return CoreStatsValues;
}
