// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutoSaveData.h"
#include "Features/Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "SaveManagerData.generated.h"

USTRUCT(BlueprintType)
struct FSaveManagerData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<UDefaultSaveGame> SaveGameClass = UDefaultSaveGame::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<UDefaultSaveGame> SharedSaveGameClass = UDefaultSaveGame::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<USlotInfoItem> SlotInfoItemClass = USlotInfoItem::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<USlotInfoItem> SharedSlotInfoItemClass = USlotInfoItem::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	FAutoSaveData AutoSaveData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	bool bIsAutoSaveEnabled;
};
