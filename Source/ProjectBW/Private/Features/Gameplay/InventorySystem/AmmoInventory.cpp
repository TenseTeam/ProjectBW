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
		CurrentAmmo.Add(AmmoStack.Key, FMath::Clamp(AmmoStack.Value.InitialSize, 0, AmmoStack.Value.MaxStack));
}

int32 UAmmoInventory::GetAmmoCount(const UAmmoTypeData* AmmoType, int32& OutMaxStack) const
{
	if (!IsValid(AmmoType) || !AmmoMaxStacks.Contains(AmmoType))
		return 0;
	
	OutMaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	return CurrentAmmo[AmmoType];
}

bool UAmmoInventory::HasAmmoOfType(const UAmmoTypeData* AmmoType) const
{
	if (!IsValid(AmmoType) || !CurrentAmmo.Contains(AmmoType))
		return false;

	return CurrentAmmo[AmmoType] > 0;
}

void UAmmoInventory::AddAmmo(UAmmoTypeData* AmmoType, int32 AmountToAdd, int32& OutRemaining)
{
	if (!IsValid(AmmoType) || !CurrentAmmo.Contains(AmmoType))
		return;
	
	AmountToAdd = FMath::Abs(AmountToAdd);
	const int32 MaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	const int32& Ammo =  CurrentAmmo[AmmoType];
	OutRemaining = Ammo + AmountToAdd > MaxStack ? AmountToAdd - (MaxStack - Ammo) : 0;
	SetAmmoCount(AmmoType, Ammo + AmountToAdd);
}

void UAmmoInventory::ConsumeAmmo(UAmmoTypeData* AmmoType, int32 AmountToConsume, int32& OutConsumedAmount)
{
	AmountToConsume = FMath::Abs(AmountToConsume);
	const int32 AvailableAmmo = CurrentAmmo[AmmoType];

	if (AvailableAmmo >= AmountToConsume)
	{
		SetAmmoCount(AmmoType, AvailableAmmo - AmountToConsume);
		OutConsumedAmount = AmountToConsume;
	}
	else
	{
		SetAmmoCount(AmmoType, 0);
		OutConsumedAmount = AvailableAmmo;
	}
}

void UAmmoInventory::SetAmmoCount(UAmmoTypeData* AmmoType, const int32 NewAmount)
{
	const int32 MaxStack = AmmoMaxStacks[AmmoType].MaxStack;
	CurrentAmmo[AmmoType] = FMath::Clamp(NewAmount, 0, MaxStack);
	OnAmmoCountChanged.Broadcast(AmmoType, CurrentAmmo[AmmoType], MaxStack);
}
