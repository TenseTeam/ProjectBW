// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/QuestSystem/UObjects/Tasks/TaskBase.h"
#include "Features/Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "SingleTask.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class VUNDK_API USingleTask : public UTaskBase
{
	GENERATED_BODY()

public:
	virtual void AchieveTask(bool bFullyAchieve) override;
};
