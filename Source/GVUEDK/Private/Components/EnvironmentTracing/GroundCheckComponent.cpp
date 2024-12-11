// Copyright Villains, Inc. All Rights Reserved.


#include "Components/EnvironmentTracing/GroundCheckComponent.h"


UGroundCheckComponent::UGroundCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UGroundCheckComponent::CheckGroundDistance() const
{
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start + FVector::DownVector * MaxGroundDistance;

	FHitResult HitResult;
	if (PerformSphereTrace(Start, End, SphereCastRadius, HitResult, GroundTraceChannel))
	{
		return (Start - HitResult.ImpactPoint).Size();
	}
	return MaxGroundDistance;
}

