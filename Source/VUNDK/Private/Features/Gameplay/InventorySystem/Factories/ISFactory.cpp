// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Factories/ISFactory.h"

#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "Features/Gameplay/InventorySystem/Base/Data/ItemDataBase.h"
#include "Features/Gameplay/InventorySystem/Base/ItemDropActor.h"

UItemBase* UISFactory::CreateItem(UItemDataBase* Data)
{
	if (Data == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Data in CreateItem() is nullptr."));
		return nullptr;
	}
	
	if (TSubclassOf<UItemBase> ItemClass = Data->ItemClass; ItemClass == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item Class in CreateItem() is nullptr, using default ItemBase class."));
		ItemClass = UItemBase::StaticClass();
	}
	
	UItemBase* Item = NewObject<UItemBase>(GetTransientPackage(), Data->ItemClass);
	Item->Init(Data);
	return Item;
}

UItemBase* UISFactory::CreateItemInQuantity(UItemDataBase* Data, int32& OutOverflow, const int32 Quantity)
{
	UItemBase* Item = CreateItem(Data);

	if (Item == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item in CreateItemInQuantity() is nullptr."));
		return nullptr;
	}

	OutOverflow = Item->SetQuantity(Quantity);
	return Item;
}

AItemDropActor* UISFactory::CreateItemDropActor(UItemBase* Item, const APlayerController* PlayerController)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Item in CreateItemDropActor() is not valid."));
		return nullptr;
	}

	if (!IsValid(PlayerController))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("PlayerController in CreateItemDropActor() is not valid."));
		return nullptr;
	}

	if (!IsValid(PlayerController->GetWorld()))
	{
		UE_LOG(LogInventorySystem, Error, TEXT("World in CreateItemDropActor() is not valid."));
		return nullptr;
	}
	
	AItemDropActor* ItemDropActor = Cast<AItemDropActor>(PlayerController->GetWorld()->SpawnActor(Item->GetItemDropActorClass()));
	ItemDropActor->Init(Item);
	return ItemDropActor;
}
