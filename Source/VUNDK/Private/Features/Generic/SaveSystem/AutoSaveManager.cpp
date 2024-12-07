// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Generic/SaveSystem/AutoSaveManager.h"
#include "Features/Generic/SaveSystem/SaveManager.h"
#include "Features/Generic/SaveSystem/Constants/SaveConstants.h"
#include "Features/Generic/SaveSystem/Utility/SSSlotsUtility.h"

USaveManager* UAutoSaveManager::CurrentSaveManager = nullptr;
FAutoSaveData* UAutoSaveManager::AutoSaveData = nullptr;
FTimerHandle UAutoSaveManager::AutoSaveTimerHandle;
bool UAutoSaveManager::bIsPaused = false;

void UAutoSaveManager::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
	AutoSaveData = &SaveManager->AutoSaveData;

	AutoSaveTimerHandle = FTimerHandle();
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateStatic(&UAutoSaveManager::AutoSave);

	if (FTimerManager* TimerManager = GetWorldTimerManager())
	{
		TimerManager->SetTimer(AutoSaveTimerHandle, TimerDelegate, AutoSaveData->AutoSaveIntervalSeconds, true);
		if (AutoSaveData->bInitPauseState)
			PauseAutoSave();
		else
			UnpauseAutoSave();
	}
	else
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Auto Save Failed to Initialize: World Timer Manager is not valid."));
	}
}

void UAutoSaveManager::AutoSave()
{
	if (bIsPaused) return; // Safe check, should not happen since the timer is paused

	const int32 CurrentAutoSavesNumber = USSSlotsUtility::GetTotalAutoSaveSlots(); 
	FString AutoSaveSlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(CurrentAutoSavesNumber);
	if (CurrentAutoSavesNumber >= AutoSaveData->MaxAutoSaves)
	{
		FSlotInfoData OutSlotData;
		USSSlotsUtility::TryGetMostAncientSlotInfoData(OutSlotData, ESaveTypeFilter::Auto);
		AutoSaveSlotName = OutSlotData.SlotInfoName;
	}

	CurrentSaveManager->Save(AutoSaveSlotName, CurrentSaveManager);
}

void UAutoSaveManager::PauseAutoSave()
{
	bIsPaused = true;
	GetWorldTimerManager()->PauseTimer(AutoSaveTimerHandle);
}

void UAutoSaveManager::UnpauseAutoSave()
{
	bIsPaused = false;
	GetWorldTimerManager()->UnPauseTimer(AutoSaveTimerHandle);
}

bool UAutoSaveManager::IsAutoSavePaused()
{
	return bIsPaused;
}

FTimerManager* UAutoSaveManager::GetWorldTimerManager()
{
	if (!GEngine)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("AutoSave Timer GEngine is not valid."));
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(CurrentSaveManager, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("AutoSave Timer World is not valid."));
		return nullptr;
	}

	return &World->GetTimerManager();
}
