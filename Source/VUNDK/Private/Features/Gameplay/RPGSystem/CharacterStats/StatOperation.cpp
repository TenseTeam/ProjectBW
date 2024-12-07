// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/RPGSystem/CharacterStats/StatOperation.h"

void UStatOperation::Init(UObject* InPayload, int32 InModifierValue)
{
	Payload = InPayload;
	ModifierValue = InModifierValue;
}

int32 UStatOperation::GetResultOperation_Implementation() const
{
	return ModifierValue / 2;
}
