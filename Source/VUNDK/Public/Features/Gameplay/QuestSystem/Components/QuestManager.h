// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "Features/Gameplay/QuestSystem/Data/DataAssets/QuestData.h"
#include "Features/Gameplay/QuestSystem/Data/DataAssets/QuestLogData.h"
#include "Features/Gameplay/QuestSystem/Data/Structs/QuestLogSaveData.h"
#include "QuestManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyQuestCompleted,
	const UQuestBase*, Quest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyTaskAchieved,
	const UQuestBase*, Quest,
	const UTaskBase*, Task);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnQuestTracked,
	const UQuestBase*, Quest);

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UQuestLogData* QuestLogData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestBase* TrackedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<UQuestData*, UQuestBase*> AllQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> ActiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> InactiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestBase*> CompletedQuests;
	UPROPERTY(BlueprintAssignable)
	FOnAnyQuestCompleted OnAnyQuestCompleted;
	UPROPERTY(BlueprintAssignable)
	FOnAnyTaskAchieved OnAnyTaskAchieved;
	UPROPERTY(BlueprintAssignable)
	FOnQuestTracked OnQuestTracked;

public:
	UQuestManager();

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	virtual FQuestLogSaveData CreateSaveData();

	UFUNCTION(BlueprintCallable)
	virtual void LoadSaveData(FQuestLogSaveData QuestLogSaveData);

	UFUNCTION(BlueprintCallable)
	void TrackQuest(const UQuestData* QuestDataKey);

	UFUNCTION(BlueprintCallable)
	void AchieveTaskInActiveQuests(const UTaskData* TaskDataKey);

	UFUNCTION(BlueprintCallable)
	void AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddToActiveQuests(const UQuestData* QuestDataKey);

	UFUNCTION(BlueprintCallable)
	void AddToInactiveQuests(const UQuestData* QuestDataKey);

	UFUNCTION(BlueprintCallable)
	void AddToCompletedQuests(const UQuestData* QuestDataKey, bool bAchieveAllTasks = false);

	UFUNCTION(BlueprintCallable)
	bool IsInCompletedQuestsList(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable)
	bool IsInActiveQuestsList(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable)
	bool IsInInactiveQuestsList(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable)
	bool IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable)
	UQuestBase* GetQuest(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable)
	TArray<UQuestBase*> GetQuestsByFilter(const UQuestFilterData* QuestFilterData) const;

	void ResetQuestLog();
	
	UQuestBase* GetQuestByFName(const FName QuestFName) const;
	
	void TrackQuestByFName(const FName QuestFName);

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	void LogAllQuests() const;
#endif

protected:
	virtual void BeginPlay() override;
	
	void AddQuest(UQuestData* QuestData, const FQuestEntryData QuestEntryData);
	
	void RemoveQuest(const UQuestData* QuestDataKey);
};
