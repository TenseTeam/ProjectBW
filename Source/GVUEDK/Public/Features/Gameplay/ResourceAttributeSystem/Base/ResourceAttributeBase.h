// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ResourceAttributeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIncreasedAttribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDecreasedAttribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachedMaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachedMinValue);

UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class GVUEDK_API UResourceAttributeBase : public UObject
{
	GENERATED_BODY()

public:
	UResourceAttributeBase();
	virtual void Initialize(AActor* InOwner, const FName InName);

	UFUNCTION(BlueprintCallable)
	float GetCurrentValue() const { return CurrentValue; }
	UFUNCTION(BlueprintCallable)
	virtual void SetValue(const float NewValue);
	UFUNCTION(BlueprintCallable)
	virtual void AddValue(const float Value);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveValue(const float Value);

private:
	void StartRegen();
	void StopRegen();
	void Regen();
	void CheckOrder(float& InMinValue, float& InMaxValue);

public:
	UPROPERTY(BlueprintAssignable)
	FIncreasedAttribute OnIncreasedAttribute;
	UPROPERTY(BlueprintAssignable)
	FDecreasedAttribute OnDecreasedAttribute;
	UPROPERTY(BlueprintAssignable)
	FReachedMaxValue OnReachedMaxValue;
	UPROPERTY(BlueprintAssignable)
	FReachedMinValue OnReachedMinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRegen"))
	bool bStopRegenOnValueDecrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRegen"))
	float RegenSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRegen"))
	float RegenDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRegen", ToolTip = "Regen starts when the current value is below this threshold"))
	float StartRegenValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRegen", ToolTip = "Regen stops when the current value is above this threshold"))
	float StopRegenValue;

protected:
	UPROPERTY()
	AActor* Owner;
	
	FName Name;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentValue;

private:
	bool bMustRegen;
	FTimerHandle RegenTimer;
	FTimerHandle RegenDelayTimer;
	

	
};
