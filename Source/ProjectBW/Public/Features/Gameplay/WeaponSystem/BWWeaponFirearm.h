// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Items/BWWeaponFirearmItem.h"
#include "Features/Gameplay/WeaponSystem/WeaponFirearm.h"
#include "BWWeaponFirearm.generated.h"

class UWidgetComponent;
DEFINE_LOG_CATEGORY_STATIC(LogBWWeapons, All, All);

UCLASS()
class PROJECTBW_API ABWWeaponFirearm : public AWeaponFirearm
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBWWeaponFirearmItem* WeaponFirearmItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* AmmoWidgetComponent;
	
public:
	ABWWeaponFirearm();

	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr) override;

	UFUNCTION(BlueprintPure)
	UBWWeaponFirearmItem* GetWeaponFirearmItem() const;
	
protected:
	virtual void OnCurrentAmmoChanged_Implementation(int32 CurrentAmmo, int32 MagSize) override;
};
