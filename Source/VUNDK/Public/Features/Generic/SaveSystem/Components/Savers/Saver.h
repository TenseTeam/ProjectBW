// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Features/Generic/SaveSystem/Data/Saves/DefaultSaveGame.h"
#include "Features/Generic/SaveSystem/SaveManager.h"
#include "Saver.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API USaver : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSave OnPrepSave;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareLoad OnPrepLoad;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSaveGame OnSaveGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnLoadGame OnLoadGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSharedSave OnPrepSharedSave;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnPrepareSharedLoad OnPrepSharedLoad;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSharedSaveGame OnSharedSaveGameCompleted;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Generic|Save System")
	FOnSharedLoadGame OnSharedLoadGameCompleted;
	
public:
	USaver();
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	FName GetUniqueSaveID() const;
	
protected:
	UFUNCTION()
	void PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);
	
	UFUNCTION()
	void PrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION()
	void PrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION()
	void PrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnPrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnPrepareLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnPrepareSharedSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnPrepareSharedLoad(UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnSharedSaveCompletedEvent(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame, UObject* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "VUNDK|Generic|Save System")
	void OnSharedLoadCompletedEvent(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator);
	
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
