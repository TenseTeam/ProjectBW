// Copyright VUDNK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Features/Gameplay/DismembermentSystem/Data/BloodDecalSpawnParams.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DismemberUtility.generated.h"

UCLASS()
class VUNDK_API UDismemberUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FBloodDecalSpawnParams& BloodDecalSpawnParams);

	static UNiagaraComponent* SpawnBloodExplosionFX(const UWorld* World, UNiagaraSystem* BloodExplosionFX, const FVector& ExplosionLocation, const FName& NiagaraCallbackName, UObject* CallbackObject);
	
	static UNiagaraComponent* SpawnBloodSpillFX(const UWorld* World, UNiagaraSystem* BloodSpillFX, USceneComponent* AttachToComponent, const FName& AttachToBoneName, const FVector& SprayDirection, const FName& NiagaraCallbackName, UObject* CallbackObject);
};
