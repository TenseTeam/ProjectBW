// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/RPGInventoryItemsSaveData.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventory.h"
#include "RPGInventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API URPGInventory : public UTetrisInventory
{
	GENERATED_BODY()
	
public:
	URPGInventory();

protected:
	virtual USaveData* CreateSaveDataObject_Implementation() override;

	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	void CreateRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData, TArray<UItemBase*>& ItemsToSave);

	void LoadRPGInventorySaveData(FRPGInventoryItemsSaveData& RPGItemsSaveData);
};
