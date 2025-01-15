// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/ShootPoint.h"

UShootPoint::UShootPoint()
{
	PrimaryComponentTick.bCanEverTick = false;
#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(this);
	ArrowComponent->ArrowSize = 0.5f;
#endif
}

FVector UShootPoint::GetShootPointLocation() const
{
	return GetComponentLocation();
}

FVector UShootPoint::GetShootPointDirection() const
{
	return GetForwardVector();
}
