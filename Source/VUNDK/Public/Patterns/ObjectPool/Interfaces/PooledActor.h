// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PooledActor.generated.h"

UINTERFACE(BlueprintType)
class UPooledActor : public UInterface
{
	GENERATED_BODY()
};

class VUNDK_API IPooledActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorBeginPlay();

	UFUNCTION(BlueprintNativeEvent)
	void OnPooledActorEndPlay();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AssignActorPool(class UActorPool* InActorPool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UActorPool* GetActorPool() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReleasePooledActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearPooledActor();
};
