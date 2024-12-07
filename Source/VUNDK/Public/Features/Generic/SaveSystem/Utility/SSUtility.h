// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Generic/SaveSystem/SaveManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSUtility.generated.h"

UCLASS()
class VUNDK_API USSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	
public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static USaveManager* GetSaveManager();
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static UDefaultSaveGame* GetSaveGame();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static UDefaultSaveGame* GetSharedSaveGame();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void StartNewSaveGame();
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot = false);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void SharedSave(UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void ManualAndSharedSave(UObject* Instigator, bool bOverrideCurrentSlot = false);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void SelectAndSaveOnSlot(const FString& SlotName, UObject* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void LoadSelectedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void LoadSharedSlot(UObject* Instigator);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void LoadSelectedSlotAndSharedSlot(UObject* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void StartTimePlayedTimer();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static bool HasInstanceEverLoaded();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static bool HasInstanceEverSaved();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static bool HasSharedInstanceEverLoaded();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static bool HasSharedInstanceEverSaved();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void SetSaveMasterID(const FName& SaveMasterID);
	
	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static FName GetSaveMasterID();
	
private:
	static bool Check();
};
