// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "Components/ActorComponent.h"
#include "PoolsManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UPoolsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TMap<FName, UActorPool*> Pools;

public:
	UPoolsManager();

	UFUNCTION(BlueprintPure)
	UActorPool* GetPool(FName PoolName) const;

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	void InitPools();

	void DestroyPools();
};
