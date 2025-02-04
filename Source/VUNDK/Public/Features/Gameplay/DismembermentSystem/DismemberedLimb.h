// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "DismemberedLimb.generated.h"

class UDismemberer;

constexpr float BoneScale = 0.00001f;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class VUNDK_API ADismemberedLimb : public AActor
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
	FName TargetBoneName;
	TArray<FName> LimbBoneNames;
	
public:
	ADismemberedLimb();
	
	void Init(UDismemberer* InDismemberer, const FName BoneName, const FVector Impulse);

	TArray<FName> GetLimbBoneNames() const;
	
private:
	void IsolateLimb();
	
	void SetLimbActorLocationAndRotation();
	
	FName GetRootBoneName() const;
	
	UPrimitiveComponent* CreateApproximateCollision();

	bool Check() const;
};
