// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/UI/BetterDrag/BetterDragDropOperation.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterDragDropOperationFactory.generated.h"

UCLASS()
class VUNDK_API UBetterDragAndDropFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUNDK|UI|BetterDrag")
	static UBetterDragDropOperation* CreateBetterDragDropOperation(const TSubclassOf<UBetterDragDropOperation> DragDropOperationClass, UUserWidget* OwnerWidget, UUserWidget* DragVisual, UObject* Payload, const ESlateVisibility OwnerWidgetVisibility, const FKey DropKey);
};
