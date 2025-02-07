// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/Base/ItemDropActor.h"
#include "RPGItemVisualDetails.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemVisualDetails
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* ItemMesh;

    FRPGItemVisualDetails(): ItemMesh(nullptr)
	{
	}

	bool operator==(const FRPGItemVisualDetails& Other) const
	{
		return Name.EqualTo(Other.Name) &&
			   Icon.GetResourceObject() == Other.Icon.GetResourceObject() &&
			   Description.EqualTo(Other.Description) &&
			   ItemMesh == Other.ItemMesh;
	}
};

FORCEINLINE uint32 GetTypeHash(const FRPGItemVisualDetails& Item)
{
	const uint32 Hash = FCrc::MemCrc32(&Item, sizeof(FRPGItemVisualDetails));
	return Hash;
}
