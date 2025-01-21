// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/ResourceAttributeSystem/HealthAttribute.h"


void UHealthAttribute::SetValue(const float NewValue)
{
	if (IsInvulnerable()) return;
	Super::SetValue(NewValue);
}

bool UHealthAttribute::IsDead() const
{
	return CurrentValue <= 0.f;
}

bool UHealthAttribute::IsInvulnerable() const
{
	return bIsInvulnerable;
}

void UHealthAttribute::SetInvulnerable(const bool Value)
{
	bIsInvulnerable = Value;
}
