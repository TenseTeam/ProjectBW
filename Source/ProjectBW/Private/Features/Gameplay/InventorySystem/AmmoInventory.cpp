// Copyright Villains, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/AmmoInventory.h"

TMap<FGuid, int32> UAmmoInventory::CreateAmmoSaveData() const
{
	TMap<FGuid, int32> AmmoSaveData;
	
	for (auto& Ammo : CurrentAmmo)
		AmmoSaveData.Add(Ammo.Key->AmmoID, Ammo.Value);
	
	return AmmoSaveData;
}

void UAmmoInventory::LoadAmmoSaveData(const TMap<FGuid, int32>& AmmoSaveData)
{
	for (auto& Ammo : AmmoSaveData)
	{
		UAmmoTypeData* AmmoType = nullptr;
		for (const auto& AmmoStack : AmmoMaxStacks)
		{
			if (AmmoStack.Key->AmmoID == Ammo.Key)
			{
				AmmoType = AmmoStack.Key;
				break;
			}
		}
		
		if (IsValid(AmmoType))
		{
			CurrentAmmo.Add(AmmoType);
			SetAmmoCount(AmmoType, Ammo.Value);
		}
	}
}

void UAmmoInventory::Init()
{
	for (auto& AmmoStack : AmmoMaxStacks)
		CurrentAmmo.Add(AmmoStack.Key, AmmoStack.Value.InitialSize);
}

int32 UAmmoInventory::GetAmmoCount(const UAmmoTypeData* AmmoType, int32& OutMaxStack) const
{
	if (!IsValid(AmmoType) || !AmmoMaxStacks.Contains(AmmoType))
		return 0;
	
	OutMaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	return CurrentAmmo[AmmoType];
}

bool UAmmoInventory::CanRemoveAmmo(const UAmmoTypeData* AmmoType, const int32 Amount) const
{
	if (!IsValid(AmmoType))
		return false;

	if (!CurrentAmmo.Contains(AmmoType))
		return false;

	return CurrentAmmo[AmmoType] - Amount >= 0;
}

bool UAmmoInventory::TryAddAmmo(UAmmoTypeData* AmmoType, int32 Amount, int32& OutRemaining)
{
	if (!IsValid(AmmoType))
		return false;

	if (!CurrentAmmo.Contains(AmmoType))
		return false;
	
	Amount = FMath::Abs(Amount);
	const int32 MaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	
	const int32& Ammo =  CurrentAmmo[AmmoType];
	OutRemaining = Ammo + Amount > MaxStack ? Amount - (MaxStack - Ammo) : 0;
	SetAmmoCount(AmmoType, Ammo + Amount);
	return true;
}

bool UAmmoInventory::TryRemoveAmmo(UAmmoTypeData* AmmoType, int32 Amount)
{
	if (!CanRemoveAmmo(AmmoType, Amount))
		return false;

	Amount = FMath::Abs(Amount);
	const int32& Ammo = CurrentAmmo[AmmoType];
	SetAmmoCount(AmmoType, Ammo - Amount);
	return true;
}

void UAmmoInventory::SetAmmoCount(UAmmoTypeData* AmmoType, const int32 NewAmount)
{
	const int32 MaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	CurrentAmmo[AmmoType] = FMath::Clamp(NewAmount, 0, MaxStack); // Just in case
	OnAmmoCountChanged.Broadcast(AmmoType, CurrentAmmo[AmmoType], MaxStack);
}
