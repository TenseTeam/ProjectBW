// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberedLimb.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceExport.h"
#include "Data/BloodDecalSpawnParams.h"
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
	TArray<UNiagaraSystem*> BloodExplosionFXs;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UNiagaraSystem*> BloodSpillFXs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UMaterialInterface*> BloodStainsDecals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBloodDecalSpawnParams BloodDecalSpawnParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName PivotOrientationName = FName("neck_01");
	
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
	void DismemberLimbs(const TArray<FName>& BoneNames, const FVector Impulse);
	
	UFUNCTION(BlueprintCallable)
	void DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse);

	UFUNCTION(BlueprintCallable)
	void DismemberAllLimbs(const FVector Impulse);

	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	TArray<FName> GetDismemberedBones() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
private:
	void SpawnBloodExplosionFX(const FVector& HitLocation);
	
	void SpawnBloodSpillFX(const FName& BoneName);

	UNiagaraSystem* GetRandomBloodSpillFX() const;
	
	UNiagaraSystem* GetRandomBloodExplosionFX() const;
	
	UMaterialInterface* GetRandomBloodStainDecal() const;
	
	void SpawnDisemberedLimb(FName BoneName, const FVector Impulse);

	bool Check() const;
};
