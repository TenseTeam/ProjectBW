// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Generic/SaveSystem/Data/Saves/SlotInfoItem.h"
#include "SlotInfoData.generated.h"

USTRUCT(BlueprintType)
struct FSlotInfoData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	FString SlotInfoName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	FDateTime LastSaveDate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	double TimePlayed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	USlotInfoItem* SlotInfoItem;

	FSlotInfoData()
	{
		SlotInfoName = "";
		SlotInfoItem = nullptr;
		LastSaveDate = FDateTime::Now();
		TimePlayed = 0;
	}

	explicit FSlotInfoData(const FString& SlotName)
	{
		SlotInfoName = SlotName;
		SlotInfoItem = nullptr;
		LastSaveDate = FDateTime::Now();
		TimePlayed = 0.f;
	}

	bool operator==(const FSlotInfoData& Other) const
	{
		return SlotInfoName == Other.SlotInfoName;
	}

	bool operator<(const FSlotInfoData& Other) const
	{
		return LastSaveDate < Other.LastSaveDate;
	}

	bool operator>(const FSlotInfoData& Other) const
	{
		return LastSaveDate > Other.LastSaveDate;
	}
};
