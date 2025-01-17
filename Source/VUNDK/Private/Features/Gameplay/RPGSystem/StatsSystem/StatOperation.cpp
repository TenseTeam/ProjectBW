// Copyright VUNDK, Inc. All Rights Reserved.

#include "VUNDK/Public/Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"

void UStatOperation::Init(UObject* InPayload, const float InModifierValue)
{
	Payload = InPayload;
	ModifierValue = InModifierValue;
	OnInit();
}

void UStatOperation::OnInit_Implementation()
{
}

float UStatOperation::GetResultOperation_Implementation() const
{
	return ModifierValue / 2;
}
