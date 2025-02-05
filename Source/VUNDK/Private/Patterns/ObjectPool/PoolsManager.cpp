// Copyright VUNDK, Inc. All Rights Reserved.

#include "Patterns/ObjectPool/PoolsManager.h"

#include "Patterns/ObjectPool/Utility/PoolsUtility.h"

UPoolsManager::UPoolsManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UActorPool* UPoolsManager::GetPool(const FName PoolName) const
{
	if (!Pools.Contains(PoolName))
	{
		UE_LOG(LogTemp, Error, TEXT("Pool %s not found."), *PoolName.ToString());
		return nullptr;
	}
	
	return Pools.FindRef(PoolName);
}

void UPoolsManager::BeginPlay()
{
	Super::BeginPlay();
	UPoolsUtility::Init(this);
}
