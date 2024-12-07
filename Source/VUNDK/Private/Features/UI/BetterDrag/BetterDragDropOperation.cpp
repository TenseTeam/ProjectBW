// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/UI/BetterDrag/BetterDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetComponent.h"
#include "Features/UI/BetterDrag/BetterDragDropManager.h"
#include "Kismet/GameplayStatics.h"

UBetterDragDropOperation::UBetterDragDropOperation()
{
	bIsDragging = false;
	OwnerWidget = nullptr;
	DragVisual = nullptr;
	DropReceiver = nullptr;
	OwnerWidgetVisibility = ESlateVisibility::Hidden;
	CachedOwnerWidgetVisibility = ESlateVisibility::Hidden;
	DropKey = EKeys::LeftMouseButton;

	AddToRoot();
}

void UBetterDragDropOperation::Init(UUserWidget* InOwnerWidget, UUserWidget* InDragVisual, UObject* InPayload, ESlateVisibility InOwnerWidgetVisibility, FKey InDropKey)
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	OwnerWidget = InOwnerWidget;
	DragVisual = InDragVisual;
	Payload = InPayload;
	OwnerWidgetVisibility = InOwnerWidgetVisibility;
	DropKey = InDropKey;
	UBetterDragDropManager::SetDragDropOperation(this);
	BeginDrag();
}

void UBetterDragDropOperation::EnterReceiver(UUserWidget* InDropReceiver)
{
	if (!Check()) return;

	if (IsValid(InDropReceiver) && InDropReceiver->Implements<UBetterDragDropReceiver>())
	{
		OnDragEnter(InDropReceiver);
		IBetterDragDropReceiver::Execute_OnBetterDragEnter(InDropReceiver, this, OwnerWidget, DragVisual, Payload);
	}

	DropReceiver = InDropReceiver;
}

void UBetterDragDropOperation::LeaveReceiver()
{
	if (IsValid(DropReceiver) && DropReceiver->Implements<UBetterDragDropReceiver>())
	{
		OnDragLeave(DropReceiver);
		IBetterDragDropReceiver::Execute_OnBetterDragLeave(DropReceiver, this, OwnerWidget, DragVisual, Payload);
	}
	
	DropReceiver = nullptr;
}

void UBetterDragDropOperation::Tick(float DeltaTime)
{
	UpdateDrag(CachedSize);
}

bool UBetterDragDropOperation::IsTickable() const
{
	return bIsDragging;
}

TStatId UBetterDragDropOperation::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBetterDragDropOperation, STATGROUP_Tickables);
}

void UBetterDragDropOperation::UpdateDrag_Implementation(const FVector2D Size)
{
	if (!Check()) return;
	
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(PlayerController);
	MousePosition -= Size * FVector2D(0.5f, 0.5f);
	SetDragVisualPosition(MousePosition);
}

void UBetterDragDropOperation::SetDragVisualPosition_Implementation(const FVector2D PositionInViewport) const
{
	DragVisual->SetPositionInViewport(PositionInViewport, false);
}

void UBetterDragDropOperation::OnBeginDrag_Implementation()
{
}

void UBetterDragDropOperation::OnEndDrag_Implementation()
{
}

void UBetterDragDropOperation::OnDragEnter_Implementation(UUserWidget* EnteredWidget)
{
}

void UBetterDragDropOperation::OnDragLeave_Implementation(UUserWidget* LeftWidget)
{
}

void UBetterDragDropOperation::OnDragDrop_Implementation()
{
}

void UBetterDragDropOperation::OnDragCancelled_Implementation()
{
}

void UBetterDragDropOperation::BeginDrag()
{
	if (!Check()) return;

	int32 ZOrder = 100;
	CachedSize = OwnerWidget->GetDesiredSize();
	
	PlayerController->InputComponent->BindKey(DropKey, IE_Released, this, &UBetterDragDropOperation::CheckEndDrag);

	CachedOwnerWidgetVisibility = OwnerWidget->GetVisibility();
	OwnerWidget->SetVisibility(OwnerWidgetVisibility);
	
	if (const UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(OwnerWidget))
	{
		CachedSize = Slot->GetSize();
		ZOrder = Slot->GetZOrder() + 1;
	}
	
	DragVisual->SetDesiredSizeInViewport(CachedSize);
	DragVisual->SetVisibility(ESlateVisibility::HitTestInvisible);
	DragVisual->AddToViewport(ZOrder);
	bIsDragging = true;
	OnBeginDrag();
}

void UBetterDragDropOperation::EndDrag()
{
	if (!Check()) return;
	
	if (IsValid(DropReceiver))
	{
		OnDragDrop();
		IBetterDragDropReceiver::Execute_OnBetterDragDrop(DropReceiver, this, OwnerWidget, DragVisual, Payload);
	}
	else
	{
		OnDragCancelled();
	}
	
	OwnerWidget->SetVisibility(CachedOwnerWidgetVisibility);
	DragVisual->RemoveFromParent();

	bIsDragging = false;

	OnEndDrag();
	UBetterDragDropManager::ClearDragDropOperation();
	RemoveFromRoot();
}

void UBetterDragDropOperation::CheckEndDrag(FKey Key)
{
	if (!bIsDragging || Key != DropKey) return;

	EndDrag();
}

bool UBetterDragDropOperation::Check() const
{
	return IsValid(PlayerController) && IsValid(OwnerWidget) && IsValid(DragVisual) && DropKey.IsValid();
}
