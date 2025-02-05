// Copyright VUNDK, Inc. All Rights Reserved.

#include "Features/Gameplay/WeaponSystem/Shooter/ShootPoint.h"

#include "Features/Gameplay/WeaponSystem/Shooter/ShootBarrel.h"
#include "Features/Gameplay/WeaponSystem/Shooter/Shooter.h"

UShootPoint::UShootPoint()
{
	PrimaryComponentTick.bCanEverTick = false;
#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(this);
	ArrowComponent->ArrowSize = 0.2f;
#endif
}

void UShootPoint::SetSpread(const float NewSpread)
{
	Spread = NewSpread;
}

FVector UShootPoint::GetShootPointLocation() const
{
	return GetComponentLocation();
}

FVector UShootPoint::GetShootPointRelativeLocation() const
{
	return GetRelativeLocation();
}

FVector UShootPoint::GetShootPointDirection() const
{
	return GetForwardVector() + FMath::VRandCone(GetForwardVector(), Spread);
}

FVector UShootPoint::GetShootPointRelativeDirection() const
{
	return GetRelativeRotation().Vector() + FMath::VRandCone(GetRelativeRotation().Vector(), Spread);
}

void UShootPoint::OnRegister()
{
	Super::OnRegister();

	if (!GetAttachParent()->IsA(UShootBarrel::StaticClass()))
		UE_LOG(LogShooter, Warning, TEXT("ShootPoint %s in %s is not in a ShootBarrel."), *GetName(), *GetAttachParentActor()->GetName());
}
