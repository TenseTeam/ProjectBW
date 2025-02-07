// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskSaveData.h"
#include "Features/Gameplay/QuestSystem/Data/Enums/QuestStatus.h"
#include "QuestSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EQuestStatus QuestStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FTaskSaveData> Tasks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentTaskIndex;

	FQuestSaveData(): QuestStatus(),
	                  CurrentTaskIndex(0)
	{
	}
};
