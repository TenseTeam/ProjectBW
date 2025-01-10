// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"

#include "Utility/FGvDebug.h"


// Sets default values for this component's properties
UGvSpringArmComponent::UGvSpringArmComponent(const FObjectInitializer& ObjectInitializer)
{
	TargetArmLengthModifier = 0;
	BaseTargetArmLength = 0;

	// Enable camera controls.
	bUsePawnControlRotation = true;
}

void UGvSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
	float DeltaTime)
{
	TargetArmLength = BaseTargetArmLength + TargetArmLengthModifier;
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
	TargetArmLengthModifier = 0.0f;
}

FVector UGvSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation,
	bool bHitSomething, float DeltaTime)
{
	return Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);

	// const FVector LocalHitLocation = TraceHitLocation == FVector::ZeroVector ? FVector::ZeroVector : GetOwner()->GetTransform().InverseTransformPosition(TraceHitLocation);
	// const FVector LocalDesiredLocation = GetOwner()->GetTransform().InverseTransformPosition(DesiredArmLocation);
	// const FVector NewLocalDesiredLocation = FVector(LocalHitLocation.X, LocalDesiredLocation.Y, LocalHitLocation.Z);
	// const FVector NewWorldDesiredLocation = GetOwner()->GetTransform().TransformPosition(NewLocalDesiredLocation);
	// return bHitSomething ? NewWorldDesiredLocation : DesiredArmLocation;
}
