// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/EquipmentSystem/Data/EquipSlotKey.h"
#include "Features/Gameplay/InventorySystem/Base/ItemBase.h"
#include "ItemDataBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemTypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemBase> ItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquipSlotKey* EquipSlotKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Indicates whether the inventory is limited to a single instance of this item type id."))
	bool bIsUnique;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Indicates whether the item will be consumed upon use."))	
	bool bIsConsumable;

public:
	UItemDataBase()
	{
		ItemClass = UItemBase::StaticClass();
		EquipSlotKey = nullptr;
		ItemTypeID = FGuid::NewGuid().ToString();
		bIsUnique = false;
		bIsConsumable = false;
	}

	FORCEINLINE FName GetItemDataID() const
	{
		const FName ClassName = ItemClass ? FName(*ItemClass->GetName()) : NAME_None;
		const int32 Hash = GetTypeHash(ItemTypeID + ItemName.ToString() + ClassName.ToString());
		return FName(*FString::Printf(TEXT("ItemData_%d"), Hash));
	}
};
