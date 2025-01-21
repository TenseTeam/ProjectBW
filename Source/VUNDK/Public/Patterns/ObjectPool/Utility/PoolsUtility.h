// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Patterns/ObjectPool/PoolsManager.h"
#include "PoolsUtility.generated.h"

UCLASS()
class VUNDK_API UPoolsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UPoolsManager* PoolsManager;

public:
	static void Init(UPoolsManager* InPoolsManager);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Patterns|ObjectPool")
	static UActorPool* GetPool(FName PoolName);

private:
	static bool Check();
};
