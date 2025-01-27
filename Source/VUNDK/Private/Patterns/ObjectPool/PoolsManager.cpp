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
	InitPools();
}

void UPoolsManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	DestroyPools();
}

void UPoolsManager::InitPools()
{
	for (const auto& Pool : Pools)
		Pool.Value->Init();
}

void UPoolsManager::DestroyPools()
{
	for (const auto& Pool : Pools)
		Pool.Value->DestroyPool();
}
