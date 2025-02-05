// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "NiagaraSystem.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "DismemberedLimb.generated.h"

class UDismemberer;

constexpr float BoneScale = 0.00001f;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class VUNDK_API ADismemberedLimb : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USceneComponent* LimbRootComponent;
	UPROPERTY()
	UPoseableMeshComponent* PoseableMesh;
	UPROPERTY()
	UDismemberer* Dismemberer;
	UPROPERTY()
	USkeletalMeshComponent* TargetSkelatalMeshComponent;
	UPROPERTY()
	UNiagaraSystem* DismemberLimbFX;
	FName TargetBoneName;
	TArray<FName> LimbBoneNames;
	
public:
	ADismemberedLimb();

	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;
	
	void Init(UDismemberer* InDismemberer, FName BoneName, FVector Impulse, UNiagaraSystem* InDismemberLimbFX);

	TArray<FName> GetLimbBoneNames() const;
	
private:
	void SpawnDismemberFX();
	
	void IsolateLimb();
	
	void SetLimbActorLocationAndRotation();
	
	FName GetRootBoneName() const;
	
	UPrimitiveComponent* CreateApproximateCollision();

	bool Check() const;
};
