// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/CheckpointSystem/Components/CheckpointsManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSUtility.generated.h"

UCLASS()
class VUNDK_API UCSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UCheckpointsManager* CurrCheckpointsManager;
	
public:
	static void Init(UCheckpointsManager* CheckpointsManager);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Gameplay|Checkpoints")
	static bool TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Gameplay|Checkpoints")
	static bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

private:
	static bool Check();
};
