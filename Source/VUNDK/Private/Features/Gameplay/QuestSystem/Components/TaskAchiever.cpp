// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/QuestSystem/Components/TaskAchiever.h"
#include "Features/Gameplay/QuestSystem/Utility/QSUtility.h"

UTaskAchiever::UTaskAchiever(): TaskToAchieve(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTaskAchiever::AchieveTask()
{
	if (!TaskToAchieve)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("TaskToAchieve is null. Cannot achieve the task."));
		return;
	}
	
	UQSUtility::AchieveTask(TaskToAchieve);
}
