// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Patterns/State/StateBase.h"
#include "CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jumping")
	UCurveFloat* AscentGravityCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jumping")
	UCurveFloat* DescentGravityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging")
	float DodgeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging")
	float DodgeCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging")
	UAnimMontage* DodgeAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging")
	float DodgeAnimPlayRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging", meta = (ClampMin = 0, ClampMax = 1))
	float ExitNormalizedTime = 1.f;
};
