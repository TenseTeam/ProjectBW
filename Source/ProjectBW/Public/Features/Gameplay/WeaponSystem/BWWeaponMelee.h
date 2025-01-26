// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponBase.h"
#include "BWWeaponMelee.generated.h"

UCLASS()
class PROJECTBW_API ABWWeaponMelee : public AWeaponBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBWWeaponItem* WeaponItem;
	
public:
	ABWWeaponMelee();

	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	UFUNCTION(BlueprintPure)
	UBWWeaponItem* GetWeaponMeleeItem() const;
};
