// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/Containers/SpecialStatsContainer.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SpecialStatData.h"

void USpecialStatsContainer::AddSpecialStats(TSet<USpecialStatData*> SpecialStats)
{
	for (USpecialStatData* SpecialStat : SpecialStats)
		AddStat(SpecialStat, SpecialStat->StatMinValue);
}

TMap<USpecialStatData*, int32> USpecialStatsContainer::GetSpecialStatsValues()
{
    TMap<USpecialStatData*, int32> SpecialStatsValues;
	
    for (auto& Stat : Values)
    {
        if (USpecialStatData* SpecialStat = Cast<USpecialStatData>(Stat.Key))
            SpecialStatsValues.Add(SpecialStat, Stat.Value);
    }
    
    return SpecialStatsValues;
}
