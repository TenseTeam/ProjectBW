// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterDragDropOperation.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterDragDropManager.generated.h"

UCLASS()
class VUNDK_API UBetterDragDropManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UBetterDragDropOperation* CurrentOperation;

public:
	static void SetDragDropOperation(UBetterDragDropOperation* InOperation);

	static void ClearDragDropOperation();

	UFUNCTION(BlueprintCallable, Category = "VUNDK|UI|BetterDrag")
	static void EnterReceiver(UUserWidget* InDropReceiver);

	UFUNCTION(BlueprintCallable, Category = "VUNDK|UI|BetterDrag")
	static void LeaveReceiver();
};
