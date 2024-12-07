// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Gameplay/InventorySystem/Base/InventoryBase.h"
#include "ClassicInventory.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UClassicInventory : public UInventoryBase
{
	GENERATED_BODY()

public:
	UClassicInventory();

	virtual USaveData* CreateSaveData() override;

	virtual void LoadInventorySaveData_Implementation(UInventoryBaseSaveData* InventorySaveData) override;

	virtual void OnItemAdded_Implementation(UItemBase* Item) override;

	UFUNCTION(BlueprintPure)
	int32 GetTotalMinRequiredSlotsCount() const;

	virtual bool IsFull() const override;

	UFUNCTION(BlueprintPure)
	bool IsSatisfyingAllRequiredSlots() const;

	virtual bool CanContainItem(const UItemDataBase* ItemData) const override;
};
