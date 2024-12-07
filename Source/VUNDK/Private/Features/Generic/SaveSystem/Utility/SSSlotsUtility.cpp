// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Generic/SaveSystem/Utility/SSSlotsUtility.h"
#include "Features/Generic/SaveSystem/Constants/SaveConstants.h"
#include "Features/Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"

USaveManager* USSSlotsUtility::CurrentSaveManager = nullptr;
FString USSSlotsUtility::CurrentSlotName = "";

void USSSlotsUtility::Init(USaveManager* SaveManager)
{
	if (!SaveManager) return;

	CurrentSaveManager = SaveManager;
	TrySelectMostRecentSaveGame();
}

FString USSSlotsUtility::GetSelectedSlotName()
{
	return CurrentSlotName;
}

bool USSSlotsUtility::DoesSlotFileExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SAVES_DIRECTORY + SlotName, 0);
}

bool USSSlotsUtility::DoesSharedSlotFileExist()
{
	return DoesSlotFileExist(SHARED_SAVE_FILE_NAME);
}

bool USSSlotsUtility::DoesAnySlotFileExist(const bool bExcludeSharedSlot)
{
	return GetTotalSlots(bExcludeSharedSlot) > 0;
}

bool USSSlotsUtility::TryGetAllSaveFileNames(TArray<FString>& OutSaveFiles, const bool bWithExtension)
{
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (!FileManager.DirectoryExists(*SaveDirectory)) return false;

	FileManager.FindFiles(OutSaveFiles, *SaveDirectory, SAVE_EXTENSION);

	if (!bWithExtension)
	{
		for (FString& SlotName : OutSaveFiles)
			SlotName.RemoveFromEnd(SAVE_EXTENSION);
	}

	return OutSaveFiles.Num() > 0;
}

int32 USSSlotsUtility::GetTotalSlots(const bool bExcludeSharedSlot)
{
	TArray<FString> SaveFiles;
	if (!TryGetAllSaveFileNames(SaveFiles, false)) return 0;

	if (bExcludeSharedSlot)
		SaveFiles.Remove(SHARED_SAVE_FILE_NAME);
	
	return SaveFiles.Num();
}

int32 USSSlotsUtility::GetTotalAutoSaveSlots()
{
	TArray<FString> SaveFiles;
	int32 AutoSaveSlotsNumber = 0;
	if (!TryGetAllSaveFileNames(SaveFiles)) return 0;

	for (const FString& SlotName : SaveFiles)
	{
		if (SlotName.Contains(AUTO_SAVE_SLOT_NAME))
			AutoSaveSlotsNumber++;
	}

	return AutoSaveSlotsNumber;
}

int32 USSSlotsUtility::GetTotalManualSaveSlots()
{
	const int32 TotalSaves = GetTotalSlots();
	const int32 AutoSaveSlots = GetTotalAutoSaveSlots();
	return TotalSaves - AutoSaveSlots;
}

bool USSSlotsUtility::TryGetSharedSlotInfoData(FSlotInfoData& OutSlotInfo)
{
	TArray<FSlotInfoData> SlotInfos = CurrentSaveManager->GetSaveInfos();

	const FSlotInfoData* SharedSaveInfo = SlotInfos.FindByPredicate([&](const FSlotInfoData& SlotInfo)
	{
		return IsSharedSlotName(SlotInfo.SlotInfoName);
	});

	OutSlotInfo = *SharedSaveInfo;
	return SharedSaveInfo != nullptr;
}

bool USSSlotsUtility::TryGetSlotInfosOfType(TArray<FSlotInfoData>& OutSlotInfos, const ESaveTypeFilter Type, const bool bExcludeSharedSlot)
{
    OutSlotInfos = CurrentSaveManager->GetSaveInfos();

	if (bExcludeSharedSlot)
	{
		OutSlotInfos.RemoveAll([&](const FSlotInfoData& SlotInfo)
		{
			return SlotInfo.SlotInfoName.Contains(SHARED_SAVE_FILE_NAME);
		});
	}
	
    OutSlotInfos.RemoveAll([&](const FSlotInfoData& SlotInfo)
    {
        return (Type == ESaveTypeFilter::Manual && SlotInfo.SlotInfoName.Contains(AUTO_SAVE_SLOT_NAME)) ||
               (Type == ESaveTypeFilter::Auto && SlotInfo.SlotInfoName.Contains(SAVE_SLOT_NAME));
    });
	
    return OutSlotInfos.Num() > 0;
}

bool USSSlotsUtility::TryGetMostRecentSlotInfoData(FSlotInfoData& OutSlotData, const ESaveTypeFilter Type)
{
	TArray<FSlotInfoData> SaveInfos;

	if (!TryGetSlotInfosOfType(SaveInfos, Type)) return false;

	OutSlotData = SaveInfos[0];
	return true;
}

bool USSSlotsUtility::TryGetMostAncientSlotInfoData(FSlotInfoData& OutSlotData, const ESaveTypeFilter Type)
{
	TArray<FSlotInfoData> SaveInfos;

	if (!TryGetSlotInfosOfType(SaveInfos, Type)) return false;

	OutSlotData = SaveInfos[SaveInfos.Num() - 1];
	return true;
}

bool USSSlotsUtility::TrySelectSaveGameSlot(const FString& SlotName)
{
	if (!DoesSlotFileExist(SlotName) || IsSharedSlotName(SlotName)) return false;
	
	CurrentSlotName = SlotName;
	return true;
}

bool USSSlotsUtility::TrySelectMostRecentSaveGame()
{
	if (FSlotInfoData SlotData; TryGetMostRecentSlotInfoData(SlotData))
	{
		TrySelectSaveGameSlot(SlotData.SlotInfoName);
		return true;
	}

	return false;
}

bool USSSlotsUtility::TrySelectMostAncientSaveGame()
{
	if (FSlotInfoData SlotData; TryGetMostAncientSlotInfoData(SlotData))
	{
		TrySelectSaveGameSlot(SlotData.SlotInfoName);
		return true;
	}

	return false;
}

float USSSlotsUtility::GetMostRecentSlotInfoPlayedTime()
{
	FSlotInfoData SlotInfoData;
	if (!TryGetMostRecentSlotInfoData(SlotInfoData)) return 0.0f;
	return SlotInfoData.TimePlayed;
}

bool USSSlotsUtility::IsSharedSlot(const FSlotInfoData& SlotInfo)
{
	return IsSharedSlotName(SlotInfo.SlotInfoName);
}

bool USSSlotsUtility::IsSharedSlotName(const FString& SlotName)
{
	return SlotName == SHARED_SAVE_FILE_NAME;
}

bool USSSlotsUtility::IsSlotNameValid(const FString& SlotName)
{
	return !SlotName.IsEmpty();
}
