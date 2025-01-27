// Copyright VUNDK, Inc. All Rights Reserved.

#include "Patterns/ObjectPool/PooledActorBase.h"
#include "Patterns/ObjectPool/ActorPool.h"

APooledActorBase::APooledActorBase(): ActorPool(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APooledActorBase::AssignActorPool_Implementation(UActorPool* InActorPool)
{
	ActorPool = InActorPool;
}

UActorPool* APooledActorBase::GetActorPool_Implementation() const
{
	return ActorPool;
}

void APooledActorBase::ReleasePooledActor_Implementation()
{
	if (!Check())
	{
		UE_LOG(LogObjectPool, Warning, TEXT("APooledActorBase ReleasePooledActor(), Invalid actor pool. Destroying actor."));
		Destroy();
		return;
	}

	ActorPool->ReleaseActor(this);
}

void APooledActorBase::ClearPooledActor_Implementation()
{
}

bool APooledActorBase::Check() const
{
	return IsValid(ActorPool);
}
