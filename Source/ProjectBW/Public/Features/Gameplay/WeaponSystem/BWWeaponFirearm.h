// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "BWWeaponFirearm.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogBWWeapons, All, All);

UCLASS()
class PROJECTBW_API ABWWeaponFirearm : public AWeaponFirearm
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UBWWeaponFirearmItem* WeaponFirearmItem;
	
public:
	ABWWeaponFirearm();

	void InitBWWeapon(APawn* InOwner, UBWWeaponFirearmItem* InWeaponItem);
	
protected:
	virtual void OnReload_Implementation() override;
	
	virtual bool DeployWeaponAttack_Implementation() override;
};
