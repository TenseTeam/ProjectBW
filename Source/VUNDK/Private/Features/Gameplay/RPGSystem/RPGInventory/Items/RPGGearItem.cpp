// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGGearItem.h"
#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/RPGSystem/Factories/RPGFactory.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGGearItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"

URPGGearItem::URPGGearItem(): GearStatsContainer(nullptr)
{
}

void URPGGearItem::Init(UItemDataBase* Data)
{
	GearStatsContainer = NewObject<UCoreStatsContainer>(this, TEXT("GearStatsContainer"));
	Super::Init(Data);
}

FRPGGearItemSaveData URPGGearItem::CreateRPGGearItemSaveData() const
{
	FRPGGearItemSaveData GearSaveData;
	GearSaveData.RPGItemSaveData = CreateRPGItemSaveData();

	for (const auto& StatModifier : GearStatsContainer->GetValues())
		GearSaveData.GearStats.Add(StatModifier.Key->StatID, StatModifier.Value);

	return GearSaveData;
}

void URPGGearItem::LoadRPGGearItemSaveData(URPGInventory* LoadingInventory, FRPGGearItemSaveData& GearSaveData)
{
	for (const auto& Stats :  GearSaveData.GearStats)
	{
		if (UCoreStatData* StatData = URPGInventoriesUtility::GetStatByID(Stats.Key); IsValid(StatData))
			GearStatsContainer->AddStat(StatData, Stats.Value);
		else
			UE_LOG(LogTemp, Error, TEXT("Failed to load RPGGearItem bonus stat with ID: %s"), *Stats.Key.ToString());
	}
	
	LoadRPGItemSaveData(LoadingInventory, GearSaveData.RPGItemSaveData);
}

void URPGGearItem::AddItemStat(UCoreStatData* Stat, const TSubclassOf<UItemStatOperation> OperationClass)
{
	auto CalculateResult = [this](const UCoreStatData* LocalStat, const TSubclassOf<UItemStatOperation>& LocalOperationClass) -> int32
	{
		const UStatOperation* Operation = URPGFactory::CreateItemStatOperation(LocalOperationClass, this);
		return Operation->GetResultOperation();
	};

	if (const int32 Value = CalculateResult(Stat, OperationClass); Value != 0)
		GearStatsContainer->AddStat(Stat, Value);
}

URPGGearItemData* URPGGearItem::GetRPGGearItemData() const
{
	return Cast<URPGGearItemData>(ItemData);
}

bool URPGGearItem::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	if (!Super::CanStackItem_Implementation(OtherItem))
		return false;

	const URPGGearItem* GearItem = Cast<URPGGearItem>(OtherItem);

	if (GearItem == nullptr)
		return false;
	

	return GearStatsContainer->AreStatsEqual(GearItem->GearStatsContainer);
}
