// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/ItemDropActor.h"

AItemDropActor::AItemDropActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemDropActor::Init(class UItemBase* Item)
{
	RelatedItem = Item;
}

bool AItemDropActor::Check() const
{
	return IsValid(RelatedItem);
}
