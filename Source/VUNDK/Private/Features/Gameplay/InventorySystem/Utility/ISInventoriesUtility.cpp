// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UInventoriesManager* UISInventoriesUtility::InventoriesManager = nullptr;

void UISInventoriesUtility::Init(UInventoriesManager* Manager)
{
	InventoriesManager = Manager;
}

bool UISInventoriesUtility::Check()
{
	const bool bIsValid = InventoriesManager != nullptr;

	if (!bIsValid)
		UE_LOG(LogTemp, Error, TEXT("InventoriesManager is not valid!"));
	
	return bIsValid;
}

UInventoriesManager* UISInventoriesUtility::GetInventoriesManager()
{
	if (!Check()) return nullptr;
	
	return InventoriesManager;
}

UItemsRegistry* UISInventoriesUtility::GetRegistry()
{
	if (!Check()) return nullptr;
	
	return InventoriesManager->Registry;
}

bool UISInventoriesUtility::IsItemInRegistry(const UItemDataBase* ItemData)
{
	if (!Check()) return false;

	return InventoriesManager->IsItemInRegistry(ItemData);
}

UItemDataBase* UISInventoriesUtility::GetItemDataFromRegistry(const FGuid& ItemDataID)
{
	if (!Check()) return nullptr;

	return InventoriesManager->GetItemDataFromRegistry(ItemDataID);
}
