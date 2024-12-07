// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/QuestSystem/Data/Structs/QuestEntryData.h"
#include "QuestLogData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API UQuestLogData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UQuestData* FirstTrackedQuest;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<UQuestData*, FQuestEntryData> QuestEntries;
};
