// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContextsManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ContextsManagerUtility.generated.h"

UCLASS()
class VUNDK_API UContextsManagerUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UContextsManager* ContextsManager;
	
public:
	static void Init(UContextsManager* InContextsManager);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|IMCManagerSystem")
	static UContextsManager* GetContextsManager();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|IMCManagerSystem")
	static void SetGameplayMappingContext();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|Generic|IMCManagerSystem")
	static void SetUIMappingContext();

private:
	static bool Check();
};
