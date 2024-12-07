// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/QuestSystem/Data/Enums/TaskType.h"
#include "TaskData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UTaskData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TaskName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

public:
	UFUNCTION(BlueprintPure)
	virtual ETaskType GetTaskType() const;
};

