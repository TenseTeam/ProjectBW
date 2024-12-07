// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "..\Enums\QuestStatus.h"
#include "Features/Gameplay/QuestSystem/Data/Enums/QuestType.h"
#include "QuestEntryData.generated.h"

USTRUCT(BlueprintType)
struct FQuestEntryData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType = EQuestType::Parallel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestStatus InitialQuestStatus = EQuestStatus::Active;

	FQuestEntryData() = default;
};
