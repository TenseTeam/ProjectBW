// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Gameplay/CheckpointSystem/Data/SaveData/CheckpointsSaveData.h"
#include "CheckpointsManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogCheckpointSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckpointReached);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UCheckpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCheckpointReached OnCheckpointReached;

private:
	static TMap<FName, FTransform> CurrentCheckpoints;
	
public:
	UCheckpointsManager();
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Checkpoints")
	static FCheckpointsSaveData CreateSaveData();
	
	UFUNCTION(BlueprintCallable)
	void LoadSaveData(FCheckpointsSaveData SaveData);
	
	UFUNCTION(BlueprintCallable)
	void SetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform) const;

	UFUNCTION(BlueprintPure)
	bool TryGetMapCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform) const;

protected:
	virtual void BeginPlay() override;
};
