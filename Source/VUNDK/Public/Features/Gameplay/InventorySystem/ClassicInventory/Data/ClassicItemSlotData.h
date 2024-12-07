// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassicItemSlotData.generated.h"

USTRUCT(BlueprintType)
struct VUNDK_API FClassicItemSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StackSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotQuantity;

	bool IsMaxStacked() const
	{
		return SlotQuantity >= StackSize;
	}
};