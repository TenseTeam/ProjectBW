// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"

UItemBase* UISFactory::CreateItem(UItemDataBase* Data)
{
	if (TSubclassOf<UItemBase> ItemClass = Data->ItemClass; ItemClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item Class in CreateItem() is nullptr, using default ItemBase class."));
		ItemClass = UItemBase::StaticClass();
	}
	
	UItemBase* Item = NewObject<UItemBase>(GetTransientPackage(), Data->ItemClass);
	Item->Init(Data);
	return Item;
}
