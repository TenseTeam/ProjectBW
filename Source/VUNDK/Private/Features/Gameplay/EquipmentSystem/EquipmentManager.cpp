// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/EquipmentSystem/EquipmentManager.h"

UEquipment* UEquipmentManager::Equipment = nullptr;

void UEquipmentManager::Init(UEquipment* InEquipment)
{
	Equipment = InEquipment;
}

UEquipment* UEquipmentManager::GetEquipment()
{
	return Equipment;
}

bool UEquipmentManager::Check()
{
	return Equipment != nullptr;
}
