// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/Containers/SpecialStatsContainer.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/Data/SpecialStatData.h"

void USpecialStatsContainer::AddSpecialStats(TSet<USpecialStatData*> SpecialStats)
{
	for (USpecialStatData* SpecialStat : SpecialStats)
		AddStat(SpecialStat, SpecialStat->StatDefaultValue);
}

TMap<USpecialStatData*, float> USpecialStatsContainer::GetSpecialStatsValues()
{
    TMap<USpecialStatData*, float> SpecialStatsValues;
	
    for (auto& Stat : Values)
    {
        if (USpecialStatData* SpecialStat = Cast<USpecialStatData>(Stat.Key))
            SpecialStatsValues.Add(SpecialStat, Stat.Value);
    }
    
    return SpecialStatsValues;
}
