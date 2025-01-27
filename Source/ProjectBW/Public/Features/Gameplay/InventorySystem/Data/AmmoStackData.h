// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmmoStackData.generated.h"

USTRUCT(BlueprintType)
struct FAmmoStackData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InitialSize;
};

