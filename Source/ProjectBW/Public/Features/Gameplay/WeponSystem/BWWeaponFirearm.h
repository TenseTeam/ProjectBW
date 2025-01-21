// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/WeaponFirearmItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "BWWeaponFirearm.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogBWWeapons, All, All);

UCLASS()
class PROJECTBW_API ABWWeaponFirearm : public AWeaponFirearm
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UWeaponFirearmItem* WeaponFirearmItem;
	
public:
	ABWWeaponFirearm();

	UFUNCTION(BlueprintCallable)
	void SetWeaponFirearmItem(UWeaponFirearmItem* NewWeaponFirearmItem);

protected:
	virtual void OnReload_Implementation() override;
	
	virtual bool OnWeaponAttack_Implementation() override;
};
