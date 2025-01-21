// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/SaveData/BWInventoryItemsSaveData.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "BWInventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTBW_API UBWInventory : public URPGInventory
{
	GENERATED_BODY()

public:
	UBWInventory();

protected:
	virtual USaveData* CreateSaveDataObject_Implementation() override;
	
	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	void CreateBWInventorySaveData(FBWInventoryItemsSaveData& BWItemsSaveData, TArray<UItemBase*>& ItemsToSave);

	void LoadBWInventorySaveData(FBWInventoryItemsSaveData& BWItemsSaveData);
};
