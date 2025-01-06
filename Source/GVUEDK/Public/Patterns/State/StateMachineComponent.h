// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	friend class UStateBase;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UStateBase>> StateClasses;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", ToolTip = "If false, the owner of this component will be used as the context"))
	bool bUseExternalContext;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", EditCondition = "bUseExternalContext"))
	TSubclassOf<AActor> ContextClass;
	
	UPROPERTY()
	TArray<UStateBase*> States;
	UPROPERTY()
	AActor* Context;
	UPROPERTY()
	UStateBase* CurrentState;
	UPROPERTY()
	UStateBase* PreviousState;
	UPROPERTY()
	bool bInitialized;
	

public:
	UStateMachineComponent();
	
	UFUNCTION(BlueprintCallable)
	void ChangeState(const int Index);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStateBase* GetState(const int Index);
	UFUNCTION(Blueprintable, BlueprintPure)
	FORCEINLINE UStateBase* const GetCurrentState() const { return CurrentState; }
	UFUNCTION(Blueprintable, BlueprintPure)
	FORCEINLINE UStateBase* const GetPreviousState() const { return PreviousState; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	virtual void HandleInput(const EInputActionType InputAction, const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	
};
