// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/CharacterStats/CharacterStats.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"

UCharacterStats::UCharacterStats(): Equipment(nullptr)
{
}

void UCharacterStats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!Check())
		return;
	
	Equipment->OnEquipChanged.RemoveDynamic(this, &UCharacterStats::CalculateFullStatsValues);
}

void UCharacterStats::Init(UEquipment* InEquipment)
{
	if (!IsValid(InEquipment))
		return;
	
	Equipment = InEquipment;
	CalculateAllStatsValues();
	Equipment->OnEquipChanged.AddDynamic(this, &UCharacterStats::CalculateFullStatsValues);
}

void UCharacterStats::OnCalculateFullStatsValues_Implementation()
{
	if (!Check())
		return;
	
	for (UItemBase* Item : Equipment->GetEquippedItems())
	{
		if (const URPGGearItem* RPGItem = Cast<URPGGearItem>(Item); RPGItem != nullptr)
		{
			for (const auto& Values : GetFullStatsValues())
			{
				UStatDataBase* Stat = Values.Key;
				ModifyFullStatValue(Stat, RPGItem->GearStatsContainer->GetValueAsFloat(Stat));
			}
		}
	}
}

bool UCharacterStats::Check() const
{
	return IsValid(Equipment);
}
