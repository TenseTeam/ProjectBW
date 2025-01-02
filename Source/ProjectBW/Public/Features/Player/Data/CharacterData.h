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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hooking",
		meta = (ClampMin = 0, ClampMax = 1, ToolTip = "Normalized distance at wich the hooking end phase starts"))
	float HookEndNormalizedDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	float RotationSpeedAiming;
};
