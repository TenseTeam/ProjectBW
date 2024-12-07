// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/CharacterStats/Utility/CharacterStatsUtility.h"

UCharacterStatsBase* UCharacterStatsUtility::CharacterStatsBase = nullptr;

void UCharacterStatsUtility::Init(UCharacterStatsBase* InCharacterStatsBase)
{
	CharacterStatsBase = InCharacterStatsBase;
}

UCharacterStatsBase* UCharacterStatsUtility::GetCharacterStats()
{
	return CharacterStatsBase;
}

bool UCharacterStatsUtility::Check()
{
	return CharacterStatsBase != nullptr;
}
