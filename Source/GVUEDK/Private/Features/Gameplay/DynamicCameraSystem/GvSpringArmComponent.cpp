// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"


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
