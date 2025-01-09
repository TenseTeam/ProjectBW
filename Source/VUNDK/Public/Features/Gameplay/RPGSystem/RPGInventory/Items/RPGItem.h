// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/Rarities/RPGRarityLevelData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/Visuals//RPGItemVisualDetails.h"
#include "RPGItem.generated.h"

class URPGItemData;

UCLASS()
class VUNDK_API URPGItem : public UTetrisItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	URPGRarityLevelData* RarityLevel;
	UPROPERTY(BlueprintReadOnly)
	FRPGItemVisualDetails VisualDetails;
	
public:
	URPGItem();

	virtual void Init(UItemDataBase* Data) override;
	
	FRPGItemSaveData CreateRPGItemSaveData() const;

	void LoadRPGItemSaveData(URPGInventory* LoadingInventory, const FRPGItemSaveData& RPGItemSaveData);

	virtual FText GetItemFullName() const override;

	virtual FSlateBrush GetItemIcon() const override;

	virtual FText GetItemDescription() const override;

	virtual TSubclassOf<AItemDropActor> GetItemDropActorClass() const override;
	
	UFUNCTION(BlueprintPure)
	URPGItemData* GetRPGItemData() const;

	virtual bool CanStackItem_Implementation(UItemBase* OtherItem) const override;
};
