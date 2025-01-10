// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/MenuWidget.h"
#include "MenuManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VUNDK_API UMenuManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	bool bSwitchMappingContexts = true;
	
private:
	UPROPERTY()
	UMenuWidget* CurrentOpenMenu;
	
public:
	UFUNCTION(BlueprintCallable)
	void OpenMenu(UMenuWidget* Menu);

	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleMenu(UMenuWidget* Menu);

	UFUNCTION(BlueprintPure)
	UMenuWidget* GetCurrentOpenMenu() const;
};
