// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmmoStackData.generated.h"

USTRUCT(BlueprintType)
struct FAmmoStackData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0", EditCondition = "MaxStack > 0"))
	int32 InitialSize;

	FAmmoStackData(): MaxStack(0),
	                  InitialSize(0)
	{
	}
};
