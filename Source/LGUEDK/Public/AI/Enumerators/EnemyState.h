// Copyright Villains, Inc. All Rights Reserved.

#pragma once


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Investigating UMETA(DisplayName = "Investigating"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dead UMETA(DisplayName = "Dead"),

};
