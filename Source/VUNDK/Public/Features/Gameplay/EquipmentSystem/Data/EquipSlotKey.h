// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipSlotKey.generated.h"

UCLASS(BlueprintType, NotBlueprintable)
class VUNDK_API UEquipSlotKey : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName EquipSlotKeyID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EquipSlotName;
	
public:
	UEquipSlotKey() : EquipSlotKeyID(FName("NotGeneratedID")),
	                  EquipSlotName(FText::FromString("Equip Slot Name"))
	{
	}

	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		EquipSlotKeyID = FName(FGuid::NewGuid().ToString());
	}
};
