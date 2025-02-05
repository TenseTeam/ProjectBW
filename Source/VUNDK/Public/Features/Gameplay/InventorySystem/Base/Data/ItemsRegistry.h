// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataBase.h"
#include "Engine/DataAsset.h"
#include "ItemsRegistry.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class VUNDK_API UItemsRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UItemDataBase*> RegisteredItems;
};
