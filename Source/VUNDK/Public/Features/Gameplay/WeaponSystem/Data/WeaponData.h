// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootData.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

UCLASS()
class VUNDK_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Recoil;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FShootData ShootData;
};
