// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemVisualDetails.generated.h"

USTRUCT(BlueprintType)
struct FRPGItemVisualDetails
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
};

FORCEINLINE uint32 GetTypeHash(const FRPGItemVisualDetails& Item)
{
	const uint32 Hash = FCrc::MemCrc32(&Item, sizeof(FRPGItemVisualDetails));
	return Hash;
}
