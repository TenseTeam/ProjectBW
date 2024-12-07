// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/InventorySystem/TetrisInventory/TetrisInventory.h"
#include "RPGInventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API URPGInventory : public UTetrisInventory
{
	GENERATED_BODY()
	
public:
	URPGInventory();

	virtual USaveData* CreateSaveData() override;
	
	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;
};
