// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/RPGItemStatOperation.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatsBridgeBase.h"

void URPGItemStatOperation::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	if (RelatedItem = Cast<URPGGearItem>(Payload); RelatedItem == nullptr)
	{
		UE_LOG(LogStatsSystem, Error, TEXT("Failed to cast %s to URPGItem in %s"), *Payload->GetName(), *GetName());
		return;
	}

	GearBaseValue = RelatedItem->GetRPGGearItemData()->GearBaseMultiplier;
	RarityValue = RelatedItem->RarityLevel->RarityValue;
}
