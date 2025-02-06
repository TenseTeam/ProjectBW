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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float CrouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float SwapWeaponSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodging", meta = (ClampMin = 0))
	float StaminaDodgeCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float MinStaminaToRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jumping")
	UCurveFloat* AscentGravityCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jumping")
	UCurveFloat* DescentGravityCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hooking",
		meta = (ClampMin = 0, ClampMax = 1, ToolTip = "Normalized distance at wich the hooking end phase starts"))
	float HookEndNormalizedDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	float AimingRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = 0))
	float ReloadDuratioin;
};
