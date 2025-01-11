// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

UCLASS()
class VUNDK_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Zoom;
};
