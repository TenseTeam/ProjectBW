// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ActorComponent.h"
#include "Widgets/MenuWidget.h"
#include "MenuManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VUNDK_API UMenuManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UMenuWidget* CurrentOpenMenu;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputMappingContext* GameplayContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputMappingContext* MenuContext;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;

public:
	UFUNCTION(BlueprintCallable)
	void OpenMenu(UMenuWidget* Menu);

	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleMenu(UMenuWidget* Menu);

	UFUNCTION(BlueprintPure)
	UMenuWidget* GetCurrentOpenMenu() const;

protected:
	virtual void BeginPlay() override;

private:
	void SetGameplayMappingContext() const;

	void SetMenuMappingContext() const;
	
	bool Check() const;
};
