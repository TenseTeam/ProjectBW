// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/StatsSystem/CharacterStats/CharacterStats.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"

UCharacterStats::UCharacterStats(): Equipment(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Check())
		Equipment->OnEquipChanged.RemoveDynamic(this, &UCharacterStats::CalculateFullStatsValues);
}

void UCharacterStats::Init(UEquipment* InEquipment)
{
	if (!IsValid(InEquipment))
		return;
	
	Equipment = InEquipment;
	CalculateFullStatsValues();
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
			for (const auto& Values : FullStatsContainer->GetValues())
			{
				UStatDataBase* Stat = Values.Key;
				FullStatsContainer->TryModifyValue(Stat, RPGItem->GearStatsContainer->GetValue(Stat));
			}
		}
	}
}

bool UCharacterStats::Check() const
{
	return IsValid(Equipment);
}
