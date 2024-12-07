// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/CharacterStats/CharacterStatsBase.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Data/SaveData/StatsSaveData.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/Utility/CharacterStatsUtility.h"

UCharacterStatsBase::UCharacterStatsBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpecialStats = CreateDefaultSubobject<USpecialStatsContainer>(TEXT("SpecialStats"));
	BaseStats = CreateDefaultSubobject<UBaseStatsContainer>(TEXT("BaseStats"));

	SpecialStats->Init();
	BaseStats->Init();
	UCharacterStatsUtility::Init(this);
}

USaveData* UCharacterStatsBase::CreateSaveData()
{
	UStatsSaveData* SaveData = NewObject<UStatsSaveData>();

	TMap<UCharacterSpecialStatData*, int32> SpecialStatsMap = SpecialStats->GetDefaultValues();
	
	for (auto& Pair : SpecialStatsMap)
		SaveData->SavedSpecialStats.Add(Pair.Key->GetFName(), Pair.Value);
	
	return SaveData;
}

bool UCharacterStatsBase::LoadSaveData(USaveData* SavedData)
{
	if (!SavedData)
		return false;

	const UStatsSaveData* SaveData = Cast<UStatsSaveData>(SavedData);

	for (TMap<FName, int32> SpecialStatsMap = SaveData->SavedSpecialStats; auto& Pair : SpecialStatsMap)
	{
		if (UCharacterSpecialStatData* SpecialStatData = SpecialStats->GetSpecialStatByCodeName(Pair.Key))
			SpecialStats->SetStatDefaultValue(SpecialStatData, Pair.Value);
	}

	return true;
}
