// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActorPool.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogObjectPool, All, All);

UCLASS(NotBlueprintable, BlueprintType, EditInlineNew)
class VUNDK_API UActorPool : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 InitialSize = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCapped;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsCapped", EditConditionHides, ClampMin = "1"))
	int32 MaxSize = 100;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorClass;
	UPROPERTY()
	TArray<AActor*> AvailableActors;
	bool bHasBeenDestroyed;
	
public:
	UActorPool();

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetActorClass() const;
	
	UFUNCTION(BlueprintPure)
	AActor* AcquireActor();

	UFUNCTION(BlueprintCallable)
	void ReleaseActor(AActor* InActor);

	void SetActorClass(const TSubclassOf<AActor>& InActorClass);
	
	void DestroyPool();

	virtual void BeginDestroy() override;
	
private:
	void SpawnInstances(const int32 InCount);

	void SpawnInstance();

	void PushActor(AActor* Actor);

	AActor* PopActor();
	
	bool IsFull() const;

	bool IsEmpty() const;

	bool Check() const;

	void SetActorEnabled(AActor* Actor, bool bEnabled) const;

	static void ClearActor(AActor* Actor);
};
