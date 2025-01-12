// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Features/UI/BetterDrag/Interfaces/IBetterDragDropReceiver.h"
#include "DropReceiverWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VUNDK_API UDropReceiverWidget : public UUserWidget, public IBetterDragDropReceiver
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
