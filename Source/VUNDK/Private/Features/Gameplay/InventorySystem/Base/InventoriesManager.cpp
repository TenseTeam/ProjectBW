// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/InventoriesManager.h"
#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "Features/Gameplay/InventorySystem/Utility/ISInventoriesUtility.h"

UInventoriesManager::UInventoriesManager(): Registry(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoriesManager::IsItemInRegistry(const UItemDataBase* ItemData) const
{
	if (!Check()) return false;
	
	return Registry->RegisteredItems.Contains(ItemData);
}

UItemDataBase* UInventoriesManager::GetItemDataFromRegistry(const FName& ItemDataID) const
{
	if (!Check()) return nullptr;

	if (Registry->RegisteredItems.Num() == 0)
		return nullptr;
	
	for (UItemDataBase* ItemData : Registry->RegisteredItems)
	{
		if (!IsValid(ItemData))
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("An ItemData in the inventory registry %s is null."), *GetName());
			continue;
		}

		if (ItemData->ItemDataID == ItemDataID)
			return ItemData;
	}

	return nullptr;
}

void UInventoriesManager::BeginPlay()
{
	UISInventoriesUtility::Init(this);
	Super::BeginPlay();
}

bool UInventoriesManager::Check() const
{
	const bool bIsValid = Registry != nullptr;

	if (!bIsValid)
		UE_LOG(LogInventorySystem, Error, TEXT("Inventory registry is not valid!"));

	return bIsValid;
}
