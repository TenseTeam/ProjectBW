// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Generic/SaveSystem/SaveManager.h"
#include "Features/Generic/SaveSystem/AutoSaveManager.h"
#include "Features/Generic/SaveSystem/Constants/SaveConstants.h"
#include "Features/Generic/SaveSystem/Utility/SSSlotsUtility.h"
#include "Features/Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Features/Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"

USaveManager::USaveManager()
{
	bIsAutoSaveEnabled = false;
	CurrentInstigator = nullptr;
	SlotSaveGameInstance = nullptr;
	SharedSaveGameInstance = nullptr;
	CurrentSlotInfos = nullptr;
	CurrentSlotInfoItem = nullptr;
	CurrentSharedSlotInfoItem = nullptr;
	bIsLoading = false;
	bIsSaving = false;
	ElapsedTimePlayed = 0.f;
	bSaveAsNewGame = false;
	bHasEverLoaded = false;
	bHasEverSaved = false;
	bHasEverSharedSaved = false;
	bHasEverSharedLoaded = false;
	bIsSharedSaving = false;
	bIsSharedLoading = false;
	SaveMasterID = DEFAULT_MASTER_SAVE_ID;
}

void USaveManager::Init(const FSaveManagerData SaveManagerData)
{
	if (!SaveManagerData.SaveGameClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Game Class is not valid."));
		return;
	}

	if (!SaveManagerData.SharedSaveGameClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Shared Save Game Class is not valid."));
		return;
	}

	if (!SaveManagerData.SlotInfoItemClass)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Save Info Class is not valid."));
		return;
	}

	SaveGameClass = SaveManagerData.SaveGameClass;
	SharedSaveGameClass = SaveManagerData.SharedSaveGameClass;
	SlotInfoItemClass = SaveManagerData.SlotInfoItemClass;
	SharedSlotInfoItemClass = SaveManagerData.SharedSlotInfoItemClass;
	AutoSaveData = SaveManagerData.AutoSaveData;
	bIsAutoSaveEnabled = SaveManagerData.bIsAutoSaveEnabled;

	CreateSaveInstances();
	LoadSlotInfos(); // Load slot infos before initializing other utilities

	USSUtility::Init(this);
	USSSlotsUtility::Init(this);
	
	if (bIsAutoSaveEnabled)
		UAutoSaveManager::Init(this);

	UE_LOG(LogSaveSystem, Display, TEXT("Save System Initialized"));
}

UDefaultSaveGame* USaveManager::GetSaveGameInstance() const
{
	return SlotSaveGameInstance;
}

UDefaultSaveGame* USaveManager::GetSharedSaveGameInstance() const
{
	return SharedSaveGameInstance;
}

bool USaveManager::DeleteSlot(const FString& SlotName) const
{
	// Select the most recent slot after deleting the current one to avoid invalid selection
	if (!USSSlotsUtility::DoesSlotFileExist(SlotName)) return false;

	RemoveSlotInfo(FName(*SlotName));
	USSSlotsUtility::TrySelectMostRecentSaveGame();
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveManager::DeleteAllSlots()
{
	CreateSaveInstances();
	const FString SaveDirectory = SAVES_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();

	if (FileManager.DeleteDirectory(*SaveDirectory, false, true))
		ClearSlotInfos();
}

void USaveManager::StartNewSaveGame()
{
	if (bIsLoading || bIsSaving) return;

	CreateNewSaveInstance();
	bSaveAsNewGame = true;
	OnNewSaveGame.Broadcast();
	ManualSave(this);
}

void USaveManager::ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot)
{
	if (bOverrideCurrentSlot && USSSlotsUtility::DoesAnySlotFileExist())
	{
		Save(USSSlotsUtility::GetSelectedSlotName(), Instigator);
		return;
	}

	const int32 SaveSlots = USSSlotsUtility::GetTotalManualSaveSlots();
	const FString NextManualSlotName = SAVE_SLOT_NAME + FString::FromInt(SaveSlots);
	Save(NextManualSlotName, Instigator);
}

void USaveManager::SharedSave(UObject* Instigator)
{
	Save(SHARED_SAVE_FILE_NAME, Instigator, true);
}

void USaveManager::ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot)
{
	OnPendingSharedSave.AddDynamic(this, &USaveManager::OnPendingSharedSaveCompleted);
	ManualSave(Instigator, bOverrideCurrentSlot);
}

void USaveManager::LoadSelectedSlotAndSharedSlot(UObject* Instigator)
{
	OnPendingSharedLoad.AddDynamic(this, &USaveManager::OnPendingSharedLoadCompleted);
	USSUtility::LoadSelectedSlot(Instigator);
}

void USaveManager::LoadSelectedSlot(UObject* Instigator)
{
	if (!USSSlotsUtility::DoesAnySlotFileExist()) return;
	
	Load(USSSlotsUtility::GetSelectedSlotName(), Instigator);
}

void USaveManager::LoadSharedSlot(UObject* Instigator)
{
	Load(SHARED_SAVE_FILE_NAME, Instigator, true);
}

TArray<FSlotInfoData> USaveManager::GetSaveInfos() const
{
	TArray<FSlotInfoData> Infos;
	if (!CurrentSlotInfos) return Infos;

	CurrentSlotInfos->SlotInfos.GenerateValueArray(Infos);

	// Sort in descending order, so the most recent save is the first one
	Infos.Sort();
	Algo::Reverse(Infos);
	return Infos;
}

bool USaveManager::GetStatus(bool& OutIsLoading, bool& OutIsSaving) const
{
	OutIsLoading = bIsLoading;
	OutIsSaving = bIsSaving;
	return bIsLoading || bIsSaving;
}

bool USaveManager::HasInstanceEverSaved() const
{
	return bHasEverSaved;
}

bool USaveManager::HasInstanceEverLoaded() const
{
	return bHasEverLoaded;
}

bool USaveManager::HasSharedInstanceEverSaved() const
{
	return bHasEverSharedSaved;
}

bool USaveManager::HasSharedInstanceEverLoaded() const
{
	return bHasEverSharedLoaded;
}

void USaveManager::StartTimePlayedTimer()
{
	const UWorld* World = GetWorld();
	if (!World) return;

	ClearTimePlayed();
	World->GetTimerManager().ClearTimer(TimePlayedTimerHandle);
	World->GetTimerManager().SetTimer(TimePlayedTimerHandle, this, &USaveManager::UpdateTimePlayed,
	                                  TIME_PLAYED_TIMER_RATE, true);
}

void USaveManager::SetSaveMasterID(const FName NewSaveMasterID)
{
	SaveMasterID = NewSaveMasterID;
}

FName USaveManager::GetSaveMasterID() const
{
	return SaveMasterID;
}

void USaveManager::Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave)
{
	UDefaultSaveGame*& SaveGameInstance = bIsSharedSave ? SharedSaveGameInstance : SlotSaveGameInstance;

	if (!USSSlotsUtility::IsSlotNameValid(SaveName) ||
		!SaveGameInstance ||
		bIsLoading || bIsSaving)
		return;

	if (!Instigator)
		Instigator = this;

	bIsSharedSaving = bIsSharedSave;
	CurrentInstigator = Instigator;
	PreviousSlotNameKey = SaveGameInstance->SlotNameKey;
	const FName SlotFName = FName(*SaveName);
	SaveGameInstance->SetSlotNameKey(SlotFName);

	if (bIsSharedSave)
		CurrentSharedSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SharedSlotInfoItemClass));
	else
		CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));

	// Notify the game that it's going to save, so all the saver objects can push their data to the save game object
	if (bIsSharedSaving)
		OnPrepareSharedSave.Broadcast(SaveGameInstance, CurrentSharedSlotInfoItem, Instigator);
	else
		OnPrepareSave.Broadcast(SaveGameInstance, CurrentSlotInfoItem, Instigator);

	bIsSaving = true;
	FAsyncSaveGameToSlotDelegate AsyncSaveDelegate;
	AsyncSaveDelegate.BindUObject(this, &USaveManager::OnSaveCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SAVES_DIRECTORY + SaveName, 0, AsyncSaveDelegate);
}

void USaveManager::Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad)
{
	UDefaultSaveGame*& SaveGameInstance = bIsSharedLoad ? SharedSaveGameInstance : SlotSaveGameInstance;

	if (!USSSlotsUtility::IsSlotNameValid(SaveName) ||
		!SaveGameInstance ||
		bIsLoading || bIsSaving)
		return;

	if (!Instigator)
		Instigator = this;

	bIsSharedLoading = bIsSharedLoad;
	CurrentInstigator = Instigator;

	if (bIsSharedLoading)
		OnPrepareSharedLoad.Broadcast(SaveGameInstance, Instigator);
	else
		OnPrepareLoad.Broadcast(SaveGameInstance, Instigator);

	bIsLoading = true;
	FAsyncLoadGameFromSlotDelegate AsyncLoadDelegate;
	AsyncLoadDelegate.BindUObject(this, &USaveManager::OnLoadCompleted);
	UGameplayStatics::AsyncLoadGameFromSlot(SAVES_DIRECTORY + SaveName, 0, AsyncLoadDelegate);
}

void USaveManager::OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess)
{
	bIsSaving = false;

	if (!bSuccess)
	{
		bIsSharedSaving = false;
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to save game."))
	}
	else
	{
		bHasEverSaved = true;
		UDefaultSaveGame*& SaveGameInstance = bIsSharedSaving ? SharedSaveGameInstance : SlotSaveGameInstance;
		const FString NewSaveSlotName = SaveGameInstance->SlotNameKey.ToString();
		UpdateSlotInfo(FName(*NewSaveSlotName));
		USSSlotsUtility::TrySelectSaveGameSlot(NewSaveSlotName);

		if (bIsSharedSaving)
		{
			bIsSharedSaving = false;
			OnSharedSaveGame.Broadcast(NewSaveSlotName, UserIndex, bSuccess, SaveGameInstance, CurrentInstigator);
		}
		else
			OnSaveGame.Broadcast(NewSaveSlotName, UserIndex, bSuccess, SaveGameInstance, CurrentInstigator);

		UE_LOG(LogSaveSystem, Display, TEXT("Game saved successfully."));
	}

	bSaveAsNewGame = false;
	OnPendingSharedSave.Broadcast(CurrentInstigator);
}

void USaveManager::OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame)
{
	const bool bSuccess = SaveGame != nullptr;
	bIsLoading = false;

	if (!bSuccess)
		UE_LOG(LogSaveSystem, Error, TEXT("Failed to load game."))
	else
	{
		bHasEverLoaded = true;
		UDefaultSaveGame*& SaveGameInstance = bIsSharedLoading ? SharedSaveGameInstance : SlotSaveGameInstance;
		SaveGameInstance = Cast<UDefaultSaveGame>(SaveGame);

		if (SaveGameInstance)
		{
			const FString SlotInfoName = SaveGameInstance->SlotNameKey.ToString();

			if (bIsSharedLoading)
				OnSharedLoadGame.Broadcast(SlotInfoName, UserIndex, SaveGameInstance, CurrentInstigator);
			else
				OnLoadGame.Broadcast(SlotInfoName, UserIndex, SaveGameInstance, CurrentInstigator);

			UE_LOG(LogSaveSystem, Display, TEXT("Game loaded successfully."));
		}
		else
			UE_LOG(LogSaveSystem, Error, TEXT("Failed Cast: Loaded game is not of type %s"), *SaveGameClass->GetName());
	}

	bIsSharedLoading = false;
	OnPendingSharedLoad.Broadcast(CurrentInstigator);
}

void USaveManager::OnPendingSharedSaveCompleted(UObject* Instigator)
{
	OnPendingSharedSave.RemoveDynamic(this, &USaveManager::OnPendingSharedSaveCompleted);
	SharedSave(Instigator);
}

void USaveManager::OnPendingSharedLoadCompleted(UObject* Instigator)
{
	OnPendingSharedLoad.RemoveDynamic(this, &USaveManager::OnPendingSharedLoadCompleted);
	USSUtility::LoadSharedSlot(Instigator);
}

void USaveManager::UpdateSlotInfo(const FName NewSaveSlotNameKey)
{
	if (!CurrentSlotInfos) return;

	float CurrentPlayedTime = 0.f;
	// bIsSharedSaving is used to determine if it's a shared slot or not
	USlotInfoItem*& SlotInfoItem = bIsSharedSaving ? CurrentSharedSlotInfoItem : CurrentSlotInfoItem;
	
	if (!bSaveAsNewGame && CurrentSlotInfos->SlotInfos.Contains(PreviousSlotNameKey))
		CurrentPlayedTime = CurrentSlotInfos->SlotInfos[PreviousSlotNameKey].TimePlayed;

	if (!CurrentSlotInfos->SlotInfos.Contains(NewSaveSlotNameKey))
	{
		FSlotInfoData NewSlotInfo = FSlotInfoData(NewSaveSlotNameKey.ToString());
		NewSlotInfo.TimePlayed = CurrentPlayedTime;
		NewSlotInfo.SlotInfoItem = SlotInfoItem;
		CurrentSlotInfos->SlotInfos.Add(NewSaveSlotNameKey, NewSlotInfo);
	}

	CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].LastSaveDate = FDateTime::Now();
	CurrentSlotInfos->SlotInfos[NewSaveSlotNameKey].TimePlayed += GetElapsedTimePlayed();

	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
	UGameplayStatics::SaveGameToSlot(SlotInfoItem, META_DIRECTORY + NewSaveSlotNameKey.ToString(), 0);
}

void USaveManager::RemoveSlotInfo(const FName& SlotNameKey) const
{
	CurrentSlotInfos->SlotInfos.Remove(SlotNameKey);
	UGameplayStatics::DeleteGameInSlot(META_DIRECTORY + SlotNameKey.ToString(), 0);
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);
}

void USaveManager::ClearSlotInfos() const
{
	CurrentSlotInfos->SlotInfos.Empty();
	UGameplayStatics::SaveGameToSlot(CurrentSlotInfos, META_INFOS_PATH, 0);

	const FString FullPath = META_DIRECTORY_FULLPATH;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*META_DIRECTORY_FULLPATH, false, true);
}

void USaveManager::LoadSlotInfos()
{
	if (USlotInfos* Infos = Cast<USlotInfos>(UGameplayStatics::LoadGameFromSlot(META_INFOS_PATH, 0)))
	{
		CurrentSlotInfos = Infos;

		for (auto& SlotInfo : CurrentSlotInfos->SlotInfos)
		{
			USlotInfoItem* LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::LoadGameFromSlot(META_DIRECTORY + SlotInfo.Value.SlotInfoName, 0));
			if (!LoadedInfoItem)
			{
				TSubclassOf<USlotInfoItem> InfoItemClass = USSSlotsUtility::IsSharedSlot(SlotInfo.Value) ? SharedSlotInfoItemClass : SlotInfoItemClass;
				LoadedInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(InfoItemClass));
			}
			SlotInfo.Value.SlotInfoItem = LoadedInfoItem;
		}
	}
}

void USaveManager::CreateNewSaveInstance()
{
	SlotSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	SharedSaveGameInstance = Cast<UDefaultSaveGame>(UGameplayStatics::CreateSaveGameObject(SharedSaveGameClass));
}

void USaveManager::CreateSaveInstances()
{
	CreateNewSaveInstance();
	CurrentSlotInfos = Cast<USlotInfos>(UGameplayStatics::CreateSaveGameObject(USlotInfos::StaticClass()));
	CurrentSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SlotInfoItemClass));
	CurrentSharedSlotInfoItem = Cast<USlotInfoItem>(UGameplayStatics::CreateSaveGameObject(SharedSlotInfoItemClass));
}

void USaveManager::UpdateTimePlayed()
{
	ElapsedTimePlayed += TIME_PLAYED_TIMER_RATE;
}

void USaveManager::ClearTimePlayed()
{
	ElapsedTimePlayed = 0.f;
}

double USaveManager::GetElapsedTimePlayed()
{
	const double TimePlayed = ElapsedTimePlayed;
	ClearTimePlayed();
	return TimePlayed;
}
