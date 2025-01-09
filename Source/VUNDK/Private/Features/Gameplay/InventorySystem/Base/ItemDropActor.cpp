// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/InventorySystem/Base/ItemDropActor.h"

AItemDropActor::AItemDropActor(): RelatedItem(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AItemDropActor::Init(class UItemBase* Item)
{
	RelatedItem = Item;
	Mesh->SetStaticMesh(Item->GetItemMesh());
	Mesh->SetCollisionProfileName("OverlapAll");
}

bool AItemDropActor::Check() const
{
	return IsValid(RelatedItem);
}
