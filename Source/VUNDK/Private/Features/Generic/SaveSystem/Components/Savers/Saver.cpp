// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Generic/SaveSystem/Components/Savers/Saver.h"
#include "Features/Generic/SaveSystem/Utility/SSSlotsUtility.h"
#include "Features/Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"
#include "UGameFramework/VUNDKGameInstance.h"

USaver::USaver()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName USaver::GetUniqueSaveID() const
{
	const FName MasterID = USSUtility::GetSaveMasterID();
	const FName OwnerName = GetOwner()->GetFName();
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString UniqueID = OwnerName.ToString() + LevelName + MasterID.ToString();
	const int32 Hash = GetTypeHash(UniqueID);
	const FString Hex = FString::Printf(TEXT("%08X"), Hash);
	
	return FName(*Hex);
}

void USaver::PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	OnPrepSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSave(SaveGame, SlotInfoItem, Instigator);
}

void USaver::PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnPrepLoad.Broadcast(SaveGame, Instigator);
	OnPrepareLoad(SaveGame, Instigator);
}

void USaver::PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
	OnPrepSharedSave.Broadcast(SaveGame, SlotInfoItem, Instigator);
	OnPrepareSharedSave(SaveGame, SlotInfoItem, Instigator);
}

void USaver::PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnPrepSharedLoad.Broadcast(SaveGame, Instigator);
	OnPrepareSharedLoad(SaveGame, Instigator);
}

void USaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameInstance, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::OnPrepareSharedSave_Implementation(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator)
{
}

void USaver::OnPrepareSharedLoad_Implementation(UDefaultSaveGame* SaveGame, UObject* Instigator)
{
}

void USaver::OnSharedSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator)
{
	OnSharedSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame, Instigator);
}

void USaver::OnSharedLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator)
{
	OnSharedLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData, Instigator);
}

void USaver::BeginPlay()
{
	Super::BeginPlay();
	if (!USSUtility::GetSaveManager()) return;
	USSUtility::GetSaveManager()->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	USSUtility::GetSaveManager()->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	USSUtility::GetSaveManager()->OnSaveGame.AddDynamic(this, &USaver::OnSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnLoadGame.AddDynamic(this, &USaver::OnLoadCompletedEvent);
	USSUtility::GetSaveManager()->OnPrepareSharedSave.AddDynamic(this, &USaver::PrepareSharedSave);
	USSUtility::GetSaveManager()->OnPrepareSharedLoad.AddDynamic(this, &USaver::PrepareSharedLoad);
	USSUtility::GetSaveManager()->OnSharedSaveGame.AddDynamic(this, &USaver::OnSharedSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnSharedLoadGame.AddDynamic(this, &USaver::OnSharedLoadCompletedEvent);
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!USSUtility::GetSaveManager()) return;
	USSUtility::GetSaveManager()->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	USSUtility::GetSaveManager()->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	USSUtility::GetSaveManager()->OnSaveGame.RemoveDynamic(this, &USaver::OnSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnLoadGame.RemoveDynamic(this, &USaver::OnLoadCompletedEvent);
	USSUtility::GetSaveManager()->OnPrepareSharedSave.RemoveDynamic(this, &USaver::PrepareSharedSave);
	USSUtility::GetSaveManager()->OnPrepareSharedLoad.RemoveDynamic(this, &USaver::PrepareSharedLoad);
	USSUtility::GetSaveManager()->OnSharedSaveGame.RemoveDynamic(this, &USaver::OnSharedSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnSharedLoadGame.RemoveDynamic(this, &USaver::OnSharedLoadCompletedEvent);
}
