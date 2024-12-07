// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/RPGInventory/Items/RPGItemBase.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/RPGItemDataBase.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Utility/RPGInventoriesUtility.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGItemsRaritiesData.h"

URPGItemBase::URPGItemBase()
{
	RarityLevel = nullptr;
	VisualDetails = FRPGItemVisualDetails();
}

void URPGItemBase::Init(UItemDataBase* Data)
{
	Super::Init(Data);
	RarityLevel = URPGInventoriesUtility::GetRPGInventoriesManager()->ItemsRarities->RarityLevels.Array()[0]; // Default rarity level
}

FRPGItemSaveData URPGItemBase::CreateRPGItemSaveData() const
{
	FRPGItemSaveData RPGItemSaveData;
	RPGItemSaveData.TetrisItemSaveData = CreateTetrisSaveData();
	RPGItemSaveData.VisualDetails = VisualDetails;
	RPGItemSaveData.RarityID = RarityLevel->RarityID;
	return RPGItemSaveData;
}

void URPGItemBase::LoadRPGItemSaveData(const FRPGItemSaveData& RPGItemSaveData, URPGInventory* Inventory)
{
	VisualDetails = RPGItemSaveData.VisualDetails;
	RarityLevel = URPGInventoriesUtility::GetRarityByID(RPGItemSaveData.RarityID);
	LoadTetrisSaveData(RPGItemSaveData.TetrisItemSaveData, Inventory); // Call this after setting VisualDetails so that the item's icon is set correctly
}

FText URPGItemBase::GetItemFullName() const
{
	const FText ItemName = VisualDetails.Name.IsEmpty() ? Super::GetItemFullName() : VisualDetails.Name;

	if (RarityLevel == nullptr)
		return ItemName;
	
	return FText::Format(FText::FromString("{0} {1}"), RarityLevel->RarityName, ItemName);
}

FSlateBrush URPGItemBase::GetItemIcon() const
{
	return VisualDetails.Icon.GetResourceObject() != nullptr ? VisualDetails.Icon : Super::GetItemIcon();
}

FText URPGItemBase::GetItemDescription() const
{
	return VisualDetails.Description.IsEmpty() ? Super::GetItemDescription() : VisualDetails.Description;
}

URPGItemDataBase* URPGItemBase::GetRPGItemData() const
{
	return Cast<URPGItemDataBase>(ItemData);
}
