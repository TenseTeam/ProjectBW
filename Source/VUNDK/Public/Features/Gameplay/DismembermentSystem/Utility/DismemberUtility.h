// Copyright VUDNK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DismemberUtility.generated.h"

UCLASS()
class VUNDK_API UDismemberUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SpawnBloodDecal(const UWorld* World, UMaterialInterface* DecalMaterial, const FVector& ParticleLocation, const FVector& Size, float BloodStainDuration, float DecalSpawnChance);
	
	static UNiagaraComponent* SpawnDismemberFX(UNiagaraSystem* DismemberFX, USceneComponent* AttachToComponent, const FName& AttachToBoneName, const FVector& SprayDirection);
};
