// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemDataBase.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/StatOperation.h"
#include "RPGWeaponItemData.generated.h"

UCLASS()
class VUNDK_API URPGWeaponItemData : public URPGItemDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Tooltip = "The weapon damage operations based on the weapon level."))
	TSubclassOf<UStatOperation> WeaponDamageOperation;
};
