// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/Data/SaveData/RPGItemSaveData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisItem.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventoriesManager.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGRarityLevelData.h"
#include "Features/Gameplay/RPGSystem/RPGItemsGeneration/Data/RPGItemVisualDetails.h"
#include "RPGItemBase.generated.h"

class URPGItemDataBase;

UCLASS(Abstract)
class VUNDK_API URPGItemBase : public UTetrisItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	URPGRarityLevelData* RarityLevel;
	UPROPERTY(BlueprintReadOnly)
	FRPGItemVisualDetails VisualDetails;
	
public:
	URPGItemBase();

	virtual void Init(UItemDataBase* Data) override;
	
	FRPGItemSaveData CreateRPGItemSaveData() const;

	void LoadRPGItemSaveData(const FRPGItemSaveData& RPGItemSaveData, URPGInventory* Inventory);

	virtual FText GetItemFullName() const override;

	virtual FSlateBrush GetItemIcon() const override;

	virtual FText GetItemDescription() const override;
	
	UFUNCTION(BlueprintPure)
	URPGItemDataBase* GetRPGItemData() const;
};
