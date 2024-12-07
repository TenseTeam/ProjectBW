// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Saves/DefaultSaveGame.h"
#include "Data/Saves/SlotInfoItem.h"
#include "Data/Saves/SlotInfos.h"
#include "Data/Structs/AutoSaveData.h"
#include "Data/Structs/SaveManagerData.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SaveManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogSaveSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnPrepareSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnPrepareSharedSave,
	UDefaultSaveGame*, SaveGameData,
	USlotInfoItem*, SlotInfoItem,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPrepareSharedLoad,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnSharedSaveGame,
	const FString&, SlotName,
	const int32, UserIndex,
	bool, bSuccess,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnSharedLoadGame,
	const FString&, SlotName,
	const int32, UserIndex,
	UDefaultSaveGame*, SaveGameData,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPendingSharedSave,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPendingSharedLoad,
	UObject*, Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnNewSaveGame
	);

UCLASS(NotBlueprintable, BlueprintType)
class VUNDK_API USaveManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<UDefaultSaveGame> SaveGameClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<UDefaultSaveGame> SharedSaveGameClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<USlotInfoItem> SlotInfoItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	TSubclassOf<USlotInfoItem> SharedSlotInfoItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	bool bIsAutoSaveEnabled;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	FAutoSaveData AutoSaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VUNDK|Generic|Save System")
	double ElapsedTimePlayed;
	
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSave OnPrepareSave;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareLoad OnPrepareLoad;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSaveGame OnSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnLoadGame OnLoadGame;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnNewSaveGame OnNewSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSharedSave OnPrepareSharedSave;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSharedLoad OnPrepareSharedLoad;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSharedSaveGame OnSharedSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSharedLoadGame OnSharedLoadGame;
	
private:
	UPROPERTY()
	UDefaultSaveGame* SlotSaveGameInstance;
	UPROPERTY()
	UDefaultSaveGame* SharedSaveGameInstance;
	UPROPERTY()
	USlotInfos* CurrentSlotInfos;
	UPROPERTY()
	USlotInfoItem* CurrentSlotInfoItem;
	UPROPERTY()
	USlotInfoItem* CurrentSharedSlotInfoItem;
	UPROPERTY()
	UObject* CurrentInstigator;
	bool bIsSharedSaving;
	bool bIsSharedLoading;
	bool bIsLoading;
	bool bIsSaving;
	bool bHasEverLoaded;
	bool bHasEverSaved;
	bool bHasEverSharedSaved;
	bool bHasEverSharedLoaded;
	FName PreviousSlotNameKey;
	bool bSaveAsNewGame;
	FTimerHandle TimePlayedTimerHandle;
	FName SaveMasterID;

	FOnPendingSharedSave OnPendingSharedSave;
	FOnPendingSharedLoad OnPendingSharedLoad;

public:
	USaveManager();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void Init(const FSaveManagerData SaveManagerData);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	UDefaultSaveGame* GetSaveGameInstance() const;

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	UDefaultSaveGame* GetSharedSaveGameInstance() const;

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	bool DeleteSlot(const FString& SlotName) const;

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void DeleteAllSlots();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void StartNewSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	UFUNCTION(Blueprintable, Category = "VUNDK|Generic|Save System")
	void SharedSave(UObject* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void LoadSelectedSlotAndSharedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void LoadSelectedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void LoadSharedSlot(UObject* Instigator);
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	TArray<FSlotInfoData> GetSaveInfos() const;

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	bool GetStatus(bool& OutIsLoading, bool& OutIsSaving) const;

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	bool HasInstanceEverSaved() const;
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	bool HasInstanceEverLoaded() const;

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	bool HasSharedInstanceEverSaved() const;
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	bool HasSharedInstanceEverLoaded() const;
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void StartTimePlayedTimer();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	void SetSaveMasterID(const FName NewSaveMasterID);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	FName GetSaveMasterID() const;
	
	void Save(const FString& SaveName, UObject* Instigator, const bool bIsSharedSave = false);
	
	void Load(const FString& SaveName, UObject* Instigator, const bool bIsSharedLoad = false);
	
private:
	void OnSaveCompleted(const FString& SlotFullPathName, int32 UserIndex, bool bSuccess);
	
	void OnLoadCompleted(const FString& SlotFullPathName, int32 UserIndex, USaveGame* SaveGame);

	UFUNCTION()
	void OnPendingSharedSaveCompleted(UObject* Instigator);

	UFUNCTION()
	void OnPendingSharedLoadCompleted(UObject* Instigator);
	
	void UpdateSlotInfo(const FName NewSaveSlotNameKey);
	
	void RemoveSlotInfo(const FName& SlotNameKey) const;
	
	void ClearSlotInfos() const;
	
	void LoadSlotInfos();
	
	void CreateNewSaveInstance();
	
	void CreateSaveInstances();
	
	void UpdateTimePlayed();
	
	void ClearTimePlayed();
	
	double GetElapsedTimePlayed();
};
