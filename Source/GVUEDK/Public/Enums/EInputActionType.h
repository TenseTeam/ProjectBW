// Copyright Villains, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Roll UMETA(DisplayName = "Roll"),
	Jump UMETA(DisplayName = "Jump"),
	Run UMETA(DisplayName = "Run"),
	Shoot UMETA(DisplayName = "Shoot"),
	StopShoot UMETA(DisplayName = "StopShoot"),
	Aim UMETA(DisplayName = "Aim"),
	StopAim UMETA(DisplayName = "StopAim"),
	ThrowHook UMETA(DisplayName = "ThrowHook")
};
