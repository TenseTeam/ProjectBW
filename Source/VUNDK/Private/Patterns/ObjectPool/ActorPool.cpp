// Copyright VUNDK, Inc. All Rights Reserved.

#include "Patterns/ObjectPool/ActorPool.h"
#include "Patterns/ObjectPool/Interfaces/PooledActor.h"

UActorPool::UActorPool(): bIsCapped(false),
                          bHasBeenDestroyed(false)
{
}

void UActorPool::Init()
{
	if (!IsValid(ActorClass) || !ActorClass->ImplementsInterface(UPooledActor::StaticClass()))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool Init(), Invalid actor class."));
		return;
	}
	
	if (InitialSize < 0 || MaxSize < 0 || InitialSize > MaxSize)
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool Init(), Invalid pool size."));
		return;
	}

	SpawnInstances(InitialSize);
}

AActor* UActorPool::AcquireActor()
{
	if (IsEmpty())
	{
		if (bIsCapped)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("ActorPool AcquireActor(), Cannot acquire actor, pool is empty and capped."));
			return nullptr;
		}

		SpawnInstance();
	}
	
	return PopActor();
}

void UActorPool::ReleaseActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool ReleaseActor(), Invalid actor."));
		return;
	}

	if (AvailableActors.Contains(InActor))
		return;

	if (bHasBeenDestroyed)
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool ReleaseActor(), Cannot release actor, pool has been destroyed."));
		InActor->Destroy();
		return;
	}

	if (IsFull() && bIsCapped)
	{
		UE_LOG(LogObjectPool, Warning, TEXT("ActorPool ReleaseActor(), Cannot release actor, pool is full and capped."));
		InActor->Destroy();
		return;
	}
	
	PushActor(InActor);
}

void UActorPool::SetActorClass(const TSubclassOf<AActor>& InActorClass)
{
	if (!IsValid(InActorClass) || !InActorClass->ImplementsInterface(UPooledActor::StaticClass()))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool SetActorClass(), Invalid actor class."));
		return;
	}

	ActorClass = InActorClass;
}

void UActorPool::DestroyPool()
{
	for (AActor* Actor : AvailableActors)
		if (IsValid(Actor))
			Actor->Destroy();

	bHasBeenDestroyed = true;
}

void UActorPool::BeginDestroy()
{
	UObject::BeginDestroy();
	DestroyPool();
}

void UActorPool::SpawnInstances(const int32 InCount)
{
	int32 InstancesCount = FMath::Clamp(InCount, 0, MaxSize - AvailableActors.Num());
	
	for (int32 i = 0; i < InCount; i++)
		SpawnInstance();
}

void UActorPool::SpawnInstance()
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool AddInstance(), Invalid world."));
		return;
	}
	
	AActor* Actor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool AddInstance(), Invalid actor."));
		return;
	}

	IPooledActor::Execute_AssignActorPool(Actor, this);
	PushActor(Actor);
}

void UActorPool::PushActor(AActor* Actor)
{
	ClearActor(Actor);
	SetActorEnabled(Actor, false);
	AvailableActors.Push(Actor);
}

AActor* UActorPool::PopActor()
{
	AActor* Actor = AvailableActors.Pop();
	
	if (!IsValid(Actor))
		return nullptr;
	
	SetActorEnabled(Actor, true);
	return Actor;
}

bool UActorPool::IsFull() const
{
	return AvailableActors.Num() >= MaxSize;
}

bool UActorPool::IsEmpty() const
{
	return AvailableActors.Num() == 0;
}

bool UActorPool::Check() const
{
	return IsValid(ActorClass);
}

void UActorPool::SetActorEnabled(AActor* Actor, const bool bEnabled) const
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool SetActorEnabled(), Invalid actor."));
		return;
	}

	Actor->SetActorHiddenInGame(!bEnabled);
	Actor->SetActorEnableCollision(bEnabled);
	Actor->SetActorTickEnabled(bEnabled);

	if (bEnabled)
		IPooledActor::Execute_OnPooledActorBeginPlay(Actor);
	else
		IPooledActor::Execute_OnPooledActorEndPlay(Actor);

#if WITH_EDITOR
	const FString ActorEnabled = ActorClass->GetName().Append(bEnabled ? TEXT("_Enabled") : TEXT("_Disabled"));
	Actor->SetActorLabel(ActorEnabled, false);
	const FString FolderPathString = "Pools/" + GetFName().ToString();
	const FName FolderPathName = FName(*FolderPathString);
	Actor->SetFolderPath(FolderPathName);
#endif
}

void UActorPool::ClearActor(AActor* Actor)
{
	if (!IsValid(Actor) || !Actor->Implements<UPooledActor>())
	{
		UE_LOG(LogObjectPool, Error, TEXT("ActorPool ClearActor(), Invalid actor."));
		return;
	}

	Actor->SetActorTransform(FTransform::Identity);
	IPooledActor::Execute_ClearPooledActor(Actor);
}
