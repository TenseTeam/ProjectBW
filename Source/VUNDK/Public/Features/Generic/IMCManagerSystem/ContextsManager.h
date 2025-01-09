// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ActorComponent.h"
#include "ContextsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnGameplayMappingContextSet
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnUIMappingContextSet
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UContextsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameplayMappingContextSet OnGameplayMappingContextSet;
	UPROPERTY(BlueprintAssignable)
	FOnUIMappingContextSet OnUIMappingContextSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* GameplayInputsContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* UIInputsContext;

private:
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	
public:
	UContextsManager();

	UFUNCTION(BlueprintCallable)
	void SetGameplayMappingContext() const;

	UFUNCTION(BlueprintCallable)
	void SetUIMappingContext() const;

protected:
	virtual void BeginPlay() override;

private:
	bool Check() const;
};
