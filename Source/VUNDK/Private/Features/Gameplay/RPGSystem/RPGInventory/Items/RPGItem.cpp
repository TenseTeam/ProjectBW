// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItem.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGItemData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/Rarities/RPGItemsRaritiesData.h"

URPGItem::URPGItem()
{
	RarityLevel = nullptr;
	VisualDetails = FRPGItemVisualDetails();
}

void URPGItem::Init(UItemDataBase* Data)
{
	Super::Init(Data);

	const URPGItemsRaritiesData* RaritiesData = URPGInventoriesUtility::GetRPGInventoriesManager()->ItemsRarities;

	if (RaritiesData == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Init(), Rarities data is not set for item %s"), *GetItemFullName().ToString());
		return;
	}

	if (RaritiesData->RarityLevels.IsEmpty())
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Init(), Rarity levels are not set for item %s"), *GetItemFullName().ToString());
		return;
	}
	
	RarityLevel = RaritiesData->RarityLevels.Array()[0]; // Default rarity level
}

FRPGItemSaveData URPGItem::CreateRPGItemSaveData() const
{
	FRPGItemSaveData RPGItemSaveData;
	RPGItemSaveData.TetrisItemSaveData = CreateTetrisSaveData();
	RPGItemSaveData.VisualDetails = VisualDetails;
	
	if (IsValid(RarityLevel))
		RPGItemSaveData.RarityID = RarityLevel->RarityID;
	else
		UE_LOG(LogInventorySystem, Error, TEXT("CreateRPGItemSaveData(), Rarity level is not set for item %s"), *GetItemFullName().ToString());
		
	return RPGItemSaveData;
}

void URPGItem::LoadRPGItemSaveData(URPGInventory* LoadingInventory, const FRPGItemSaveData& RPGItemSaveData)
{
	VisualDetails = RPGItemSaveData.VisualDetails;
	RarityLevel = URPGInventoriesUtility::GetRarityByID(RPGItemSaveData.RarityID);
	LoadTetrisSaveData(LoadingInventory, RPGItemSaveData.TetrisItemSaveData); // Call this after setting VisualDetails so that the item's icon is set correctly
}

FText URPGItem::GetItemFullName() const
{
	const FText ItemName = VisualDetails.Name.IsEmptyOrWhitespace() ? Super::GetItemFullName() : VisualDetails.Name;

	if (RarityLevel == nullptr)
		return ItemName;
	
	return FText::Format(FText::FromString("{0} {1}"), RarityLevel->RarityName, ItemName);
}

FSlateBrush URPGItem::GetItemIcon() const
{
	return VisualDetails.Icon.GetResourceObject() != nullptr ? VisualDetails.Icon : Super::GetItemIcon();
}

FText URPGItem::GetItemDescription() const
{
	return VisualDetails.Description.IsEmpty() ? Super::GetItemDescription() : VisualDetails.Description;
}

TSubclassOf<AItemDropActor> URPGItem::GetItemDropActorClass() const
{
	return VisualDetails.DropActorClass != nullptr ? VisualDetails.DropActorClass : Super::GetItemDropActorClass();
}

URPGItemData* URPGItem::GetRPGItemData() const
{
	return Cast<URPGItemData>(ItemData);
}

bool URPGItem::CanStackItem_Implementation(UItemBase* OtherItem) const
{
	if (!Super::CanStackItem_Implementation(OtherItem))
		return false;
	
	const URPGItem* OtherRPGItem = Cast<URPGItem>(OtherItem);

	if (OtherRPGItem == nullptr)
		return false;

	const bool bHasSameRarity = OtherRPGItem->RarityLevel == RarityLevel;
	const bool bHasSameVisualDetails = OtherRPGItem->VisualDetails == VisualDetails;
	
	return bHasSameRarity && bHasSameVisualDetails;
}
