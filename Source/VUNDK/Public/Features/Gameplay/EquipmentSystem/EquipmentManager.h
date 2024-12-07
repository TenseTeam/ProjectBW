// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EquipmentManager.generated.h"

UCLASS()
class VUNDK_API UEquipmentManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UEquipment* Equipment;

public:
	static void Init(UEquipment* InEquipment);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|EquipmentSystem")
	static UEquipment* GetEquipment();

private:
	static bool Check();
};
