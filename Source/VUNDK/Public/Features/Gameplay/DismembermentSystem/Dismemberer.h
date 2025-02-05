// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberedLimb.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceExport.h"
#include "Dismemberer.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UDismemberer : public UActorComponent, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

	friend class ADismemberedLimb;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> DismemberablaBones;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UNiagaraSystem*> DismemberFXs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UMaterialInterface*> BloodStainsDecals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BloodStainDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BloodStainChance = 0.8f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BloodStainMinSize = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BloodStainMaxSize = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName PivotOrientationName = FName("pivot");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName NiagaraCallbackName = FName("User.Callback");

private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	TArray<FName> DismemberedBones;

public:
	UDismemberer();

	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;
	
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* InSkeletalMeshComponent);

	UFUNCTION(BlueprintCallable)
	void DismemberLimb(const FName& BoneName, const FVector Impulse);

	UFUNCTION(BlueprintCallable)
	void DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse);

	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	TArray<FName> GetDismemberedBones() const;

private:
	void SpawnDismemberFX(const FName& BoneName);
	
	UNiagaraSystem* GetRandomDismemberFX() const;

	UMaterialInterface* GetRandomBloodStainDecal() const;
	
	void SpawnDisemberedLimb(FName BoneName, const FVector Impulse);

	bool Check() const;
};
