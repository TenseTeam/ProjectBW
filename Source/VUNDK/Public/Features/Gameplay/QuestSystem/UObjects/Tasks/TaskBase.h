// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/QuestSystem/Data/DataAssets/TaskData.h"
#include "Features/Gameplay/QuestSystem/Data/Structs/TaskSaveData.h"
#include "UObject/Object.h"
#include "TaskBase.generated.h"

class UQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskAchieved);

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class VUNDK_API UTaskBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTaskData* TaskData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestBase* RelatedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAchieved;
	UPROPERTY(BlueprintAssignable)
	FOnTaskAchieved OnTaskAchieved;
	
public:
	virtual void Init(UTaskData* InitData, UQuestBase* Quest);

	UFUNCTION(BlueprintCallable)
	virtual FTaskSaveData CreateTaskSaveData() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void LoadSaveData(FTaskSaveData TaskSaveData);
	
	UFUNCTION(BlueprintCallable)
	virtual void AchieveTask(bool bFullyAchieve = false);
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetTask();

protected:
	void TriggerAchievement();
};
