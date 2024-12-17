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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SaveGameClass = UDefaultSaveGame::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDefaultSaveGame> SharedSaveGameClass = UDefaultSaveGame::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SlotInfoItemClass = USlotInfoItem::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotInfoItem> SharedSlotInfoItemClass = USlotInfoItem::StaticClass();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAutoSaveData AutoSaveData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsAutoSaveEnabled;
};
