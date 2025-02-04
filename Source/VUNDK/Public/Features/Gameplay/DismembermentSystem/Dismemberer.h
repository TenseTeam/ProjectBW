// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberedLimb.h"
#include "Components/ActorComponent.h"
#include "Dismemberer.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UDismemberer : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> DismemberablaBones;

private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	TArray<FName> DismemberedBones;

public:
	UDismemberer();

	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* InSkeletalMeshComponent);

	UFUNCTION(BlueprintCallable)
	void DismemberLimb(const FName& BoneName, const FVector Impulse);

	UFUNCTION(BlueprintCallable)
	void DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse);

	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	TArray<FName> GetDismemberedBones() const;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnDisemberedLimb(FName BoneName, const FVector Impulse);

	bool Check() const;
};
