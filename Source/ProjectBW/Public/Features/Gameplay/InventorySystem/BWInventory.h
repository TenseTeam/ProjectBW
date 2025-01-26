// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmmoInventory.h"
#include "Data/SaveData/BWInventorySaveDataWrapper.h"
#include "Features/Gameplay/RPGSystem/RPGInventory/RPGInventory.h"
#include "BWInventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTBW_API UBWInventory : public URPGInventory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UAmmoInventory* AmmoInventory;
	
public:
	UBWInventory();

protected:
	virtual void BeginPlay() override;
	
	virtual USaveData* CreateSaveDataObject_Implementation() override;
	
	virtual USaveData* CreateInventorySaveData_Implementation(USaveData* SaveData, TArray<UItemBase*>& ItemsToSave) override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	void CreateBWInventorySaveData(FBWInventorySaveDataWrapper& SaveDataWrapper, TArray<UItemBase*>& ItemsToSave) const;

	void LoadBWInventorySaveData(FBWInventorySaveDataWrapper& SaveDataWrapper);
};
