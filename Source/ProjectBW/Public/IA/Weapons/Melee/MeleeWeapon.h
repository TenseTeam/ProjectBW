// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/WeaponSystem/WeaponBase.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class PROJECTBW_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

protected:
	virtual void BeginPlay() override;

};
