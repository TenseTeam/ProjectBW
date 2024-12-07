// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QSFactory.generated.h"

UCLASS()
class VUNDK_API UQSFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UQuestBase* CreateQuestByType(UQuestData* QuestData, const FQuestEntryData& QuestEntryData);
	
	static UTaskBase* CreateTaskByType(UQuestBase* RelatedQuest, UTaskData* TaskData, const ETaskType TaskType);
};
