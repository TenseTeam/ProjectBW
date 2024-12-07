// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/QuestSystem/UObjects/Quests/QuestParallel.h"
#include "Features/Gameplay/QuestSystem/UObjects/Tasks/TaskBase.h"

void UQuestParallel::AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve)
{
	if (bIsQuestCompleted) return;
	
	UTaskBase* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	Task->AchieveTask(bFullyAchieve);
	bIsQuestCompleted = AreAllTasksAchieved();

	if (bIsQuestCompleted)
		OnQuestCompleted.Broadcast();
}
