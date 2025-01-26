// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/ResourceAttributeSystem/Base/ResourceAttributeBase.h"

#include "Utility/FGvDebug.h"

UResourceAttributeBase::UResourceAttributeBase(): bCanRegen(false), bStopRegenOnValueDecrease(true),
                                                  MaxValue(1), MinValue(0), bUseStartingValue(false), StartingValue(1),
                                                  RegenSpeed(1),
                                                  RegenDelay(1), StartRegenValue(1 - KINDA_SMALL_NUMBER), StopRegenValue(1),
                                                  CurrentValue(1), bMustRegen(false)
{
	Owner = nullptr;
}

void UResourceAttributeBase::Initialize(AActor* InOwner, const FName InName)
{
	if (!IsValid(InOwner))
	{
		FGvDebug::Error(GetName() + " needs a valid owner, initialization failed.", true);
		return;
	}
	Owner = InOwner;
	Name = InName;

	CheckOrder(MinValue, MaxValue);
	
	if (bCanRegen)
	{
		CheckOrder(StartRegenValue, StopRegenValue);
		
		if (!FMath::IsWithinInclusive(StartRegenValue, MinValue, MaxValue))
		{
			FGvDebug::Warning("Trigger regen value is not within the min and max value range, setting it to the min value.");
			StartRegenValue = MinValue;
		}
		if (!FMath::IsWithinInclusive(StopRegenValue, MinValue, MaxValue))
		{
			FGvDebug::Warning("Stop regen value is not within the min and max value range, setting it to the max value.");
			StopRegenValue = MaxValue;
		}
	}

	if (bUseStartingValue)
		SetValue(StartingValue);
	else
		SetValue(MaxValue);
}

void UResourceAttributeBase::SetValue(const float NewValue)
{
	if (CurrentValue == NewValue) return;

	const float ValueBeforeChange = CurrentValue;

	if (CurrentValue < NewValue)
	{
		CurrentValue = FMath::Min(NewValue, MaxValue);
		OnIncreasedAttribute.Broadcast();
		if (CurrentValue == MaxValue && ValueBeforeChange != MaxValue)
		{
			OnReachedMaxValue.Broadcast();
		}
		return;
	}
	
	CurrentValue = FMath::Max(NewValue, MinValue);
	OnDecreasedAttribute.Broadcast();
	if (CurrentValue == MinValue && ValueBeforeChange != MinValue)
	{
		OnReachedMinValue.Broadcast();
	}
	
	if (bCanRegen)
	{
		if (bStopRegenOnValueDecrease && bMustRegen)
		{
			StopRegen();
		}
		if (!bMustRegen && CurrentValue <= StartRegenValue)
		{
			StartRegen();
		}
	}
	
}

void UResourceAttributeBase::AddValue(const float Value)
{
	if (CurrentValue == MaxValue) return;
	const float AbsValue = FMath::Abs(Value);
	SetValue(CurrentValue + AbsValue);
}

void UResourceAttributeBase::RemoveValue(const float Value)
{
	if (CurrentValue == MinValue) return;
	const float AbsValue = FMath::Abs(Value);
	SetValue(CurrentValue - AbsValue);
}

void UResourceAttributeBase::StartRegen()
{
	bMustRegen = true;
	
	
	if (RegenDelay <= 0)
	{
		Regen();
		return;
	}
	
	// Start regen after delay
	//FTimerHandle TimerHandle;
	Owner->GetWorld()->GetTimerManager().ClearTimer(RegenDelayTimer);
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UResourceAttributeBase::Regen);
	Owner->GetWorld()->GetTimerManager().SetTimer(RegenDelayTimer, TimerDelegate, RegenDelay, false);
}

void UResourceAttributeBase::StopRegen()
{
	bMustRegen = false;
	Owner->GetWorld()->GetTimerManager().ClearTimer(RegenTimer);
}

void UResourceAttributeBase::Regen()
{
	if (!bMustRegen) return;

	if (CurrentValue == MaxValue || CurrentValue >= StopRegenValue)
	{
		CurrentValue = StopRegenValue;
		StopRegen();
		return;
	}

	const float DeltaTime = Owner->GetWorld()->GetDeltaSeconds();
	AddValue(DeltaTime * RegenSpeed);

	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UResourceAttributeBase::Regen);
	RegenTimer = Owner->GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void UResourceAttributeBase::CheckOrder(float& InMinValue, float& InMaxValue)
{
	if (InMaxValue < InMinValue)
	{
		const float Swap = InMaxValue;
		InMaxValue = InMinValue;
		InMinValue = Swap;
		FGvDebug::Warning("Max value is smaller than min value, swapping values.");
	}
	else if (InMaxValue == InMinValue)
	{
		InMaxValue++;
		FGvDebug::Warning("Max value is equal to min value, increasing max value by 1.");
	}
}
