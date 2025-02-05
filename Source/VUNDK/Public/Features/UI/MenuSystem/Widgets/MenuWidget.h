// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class VUNDK_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESlateVisibility OpenVisibility = ESlateVisibility::Visible;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESlateVisibility CloseVisibility = ESlateVisibility::Collapsed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ZOrder = 0;
	
private:
	bool bIsOpen;
	UPROPERTY()
	APlayerController* PlayerController;
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(Blueprintable)
	void Open();

	UFUNCTION(Blueprintable)
	void Close();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnOpen();

	UFUNCTION(BlueprintNativeEvent)
	void OnClose();

private:
	bool Check() const;
};
