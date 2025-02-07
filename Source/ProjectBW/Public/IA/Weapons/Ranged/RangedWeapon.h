// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "IA/Weapons/Melee/MeleeWeapon.h"
#include "RangedWeapon.generated.h"

UCLASS()
class PROJECTBW_API ARangedWeapon : public AWeaponFirearm
{
	GENERATED_BODY()

public:
	ARangedWeapon();

protected:

	virtual void BeginPlay() override;

};
