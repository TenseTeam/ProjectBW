// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/AutoSaveData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoSaveManager.generated.h"

UCLASS()
class VUNDK_API UAutoSaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static USaveManager* CurrentSaveManager;
	static FAutoSaveData* AutoSaveData;
	static FTimerHandle AutoSaveTimerHandle;
	static bool bIsPaused;

public:
	static void Init(USaveManager* SaveManager);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void PauseAutoSave();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|Save System")
	static void UnpauseAutoSave();

	UFUNCTION(BlueprintPure, Category = "VUNDK|Generic|Save System")
	static bool IsAutoSavePaused();
	
private:
	static void AutoSave();
	
	static FTimerManager* GetWorldTimerManager();
};
