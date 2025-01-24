// Copyright VUNDK, Inc. All Rights Reserved.

#include "Patterns/ObjectPool/Utility/PoolsUtility.h"

UPoolsManager* UPoolsUtility::PoolsManager = nullptr;

void UPoolsUtility::Init(UPoolsManager* InPoolsManager)
{
	PoolsManager = InPoolsManager;
}

UActorPool* UPoolsUtility::GetPool(const FName PoolName)
{
	if (!Check())
	{
		UE_LOG(LogTemp, Error, TEXT("PoolsManager is not initialized."));
		return nullptr;
	}

	return PoolsManager->GetPool(PoolName);
}

bool UPoolsUtility::Check()
{
	return IsValid(PoolsManager);
}
