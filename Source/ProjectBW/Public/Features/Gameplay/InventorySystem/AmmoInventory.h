// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/AmmoStackData.h"
#include "Features/Gameplay/WeaponSystem/Data/AmmoTypeData.h"
#include "UObject/Object.h"
#include "AmmoInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAmmoCountChanged,
	UAmmoTypeData*, AmmoType,
	int32, NewCount,
	int32, MaxStack
);

UCLASS(BlueprintType, EditInlineNew)
class PROJECTBW_API UAmmoInventory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAmmoCountChanged OnAmmoCountChanged;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<UAmmoTypeData*, FAmmoStackData> AmmoMaxStacks;

private:
	UPROPERTY()
	TMap<UAmmoTypeData*, int32> CurrentAmmo;

public:
	TMap<FGuid, int32> CreateAmmoSaveData() const;

	void LoadAmmoSaveData(const TMap<FGuid, int32>& AmmoSaveData);

	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintPure)
	int32 GetAmmoCount(const UAmmoTypeData* AmmoType, int32& OutMaxStack) const;
	
	UFUNCTION(BlueprintPure)
	bool CanRemoveAmmo(const UAmmoTypeData* AmmoType, const int32 Amount) const;
	
	UFUNCTION(BlueprintCallable)
	bool TryAddAmmo(UAmmoTypeData* AmmoType, int32 Amount, int32& OutRemaining);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveAmmo(UAmmoTypeData* AmmoType, int32 Amount);

private:
	void SetAmmoCount(UAmmoTypeData* AmmoType, int32 NewAmount);
};
