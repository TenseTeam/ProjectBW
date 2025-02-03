// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PooledActor.h"
#include "PooledActorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API APooledActorBase : public AActor, public IPooledActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UActorPool* ActorPool;

public:
	APooledActorBase();
	
protected:
	virtual void AssignActorPool_Implementation(UActorPool* InActorPool) override;

	virtual UActorPool* GetActorPool_Implementation() const override;

	virtual void ReleasePooledActor_Implementation() override;
	
	virtual void ClearPooledActor_Implementation() override;

private:
	bool Check() const;
};
