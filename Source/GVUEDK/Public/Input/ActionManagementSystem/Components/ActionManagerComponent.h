// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Input/ActionManagementSystem/PlayerAction.h"
#include "Utility/FGvDebug.h"
#include "ActionManagerComponent.generated.h"

#define CHECK_INITIALIZATION if (!bInitialized) { return; }
DECLARE_LOG_CATEGORY_EXTERN(ActionManagerLog, Log, All);

struct FInputActionValue;
class UPlayerAction;

DECLARE_DYNAMIC_DELEGATE_OneParam(FActionTriggered, const FInputActionValue&, Value);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UActionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UActionManagerComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool TryGetActioin(FGameplayTag ActionTag, UPlayerAction*& Action) const;

	UFUNCTION(BlueprintCallable)
	bool TryBindToAction(FGameplayTag ActionTag, const FActionTriggered& Callback);

	UFUNCTION(BlueprintCallable)
	bool WantPerform(FGameplayTag ActionTag) const;
	UFUNCTION(BlueprintCallable)
	bool IsPerforming(FGameplayTag ActionTag) const;
	UFUNCTION(BlueprintCallable)
	bool CanPerform(FGameplayTag ActionTag) const;

	UFUNCTION(BlueprintCallable)
	void SetCanPerform(const FGameplayTag ActionTag, const bool Value);
	UFUNCTION(BlueprintCallable)
	void SetIsPerforming(const FGameplayTag ActionTag, const bool Value);

	
	
private:
	void Initialize();
	void SetupInputComponent();

private:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, meta=(AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UPlayerAction*> PlayerActionMap;

	FGameplayTagContainer PlayerActionTags;
	
	bool bInitialized;
};


