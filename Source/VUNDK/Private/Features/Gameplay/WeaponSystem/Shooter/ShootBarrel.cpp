// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/ShootBarrel.h"
#include "Features/Gameplay/WeaponSystem/Shooter/ShootPoint.h"

UShootBarrel::UShootBarrel()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<UShootPoint*> UShootBarrel::GetShootPointsChildren() const
{
	TArray<UShootPoint*> ShootPoints;
	for (UActorComponent* Component : GetAttachChildren())
	{
		if (UShootPoint* ShootPoint = Cast<UShootPoint>(Component); IsValid(ShootPoint))
			ShootPoints.Add(ShootPoint);
	}

	return ShootPoints;
}
