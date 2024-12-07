// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UGameFramework/VUNDKControllerBase.h"
#include "Widgets/DropReceiverWidget.h"
#include "BetterDragDropOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API UBetterDragDropOperation : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* OwnerWidget;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* DragVisual;
	UPROPERTY(BlueprintReadOnly)
	UObject* Payload;
	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;
	
private:
	UPROPERTY()
	UUserWidget* DropReceiver;
	ESlateVisibility OwnerWidgetVisibility;
	ESlateVisibility CachedOwnerWidgetVisibility;
	FKey DropKey;
	bool bIsDragging;
	FVector2D CachedSize;

public:
	UBetterDragDropOperation();
	
	void Init(UUserWidget* InOwnerWidget, UUserWidget* InDragVisual, UObject* InPayload, ESlateVisibility InOwnerWidgetVisibility, FKey InDropKey);

	void EnterReceiver(UUserWidget* InDropReceiver);

	void LeaveReceiver();
	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateDrag(const FVector2D Size);

	UFUNCTION(BlueprintNativeEvent)
	void SetDragVisualPosition(const FVector2D PositionInViewport) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginDrag();

	UFUNCTION(BlueprintNativeEvent)
	void OnEndDrag();

	UFUNCTION(BlueprintNativeEvent)
	void OnDragEnter(UUserWidget* EnteredWidget);

	UFUNCTION(BlueprintNativeEvent)
	void OnDragLeave(UUserWidget* LeftWidget);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnDragDrop();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnDragCancelled();
	
private:
	void BeginDrag();
	
	void EndDrag();

	UFUNCTION()
	void CheckEndDrag(FKey Key);

	bool Check() const;
};
