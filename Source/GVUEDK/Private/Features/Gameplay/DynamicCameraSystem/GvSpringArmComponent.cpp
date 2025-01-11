// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"

#include "GameFramework/Character.h"
#include "Utility/FGvDebug.h"


// Sets default values for this component's properties
UGvSpringArmComponent::UGvSpringArmComponent(const FObjectInitializer& ObjectInitializer)
{
	TargetArmLengthModifier = 0;
	BaseTargetArmLength = 0;

	// Enable camera controls.
	bUsePawnControlRotation = true;

	OwnerCharacter = nullptr;
	bHasValidOwner = false;
}

void UGvSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	bHasValidOwner = OwnerCharacter != nullptr;
	if (!bHasValidOwner)
	{
		FGvDebug::Warning(GetName() + " Has Invalid Owner, it must be a character", true);
		return;
	}
	
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
	if (!bHasValidOwner|| !bApplyCustomBlendLocation) return Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);
	if (!bHitSomething) return DesiredArmLocation;

	// convert the desired and hit location to local space
	const FVector LocalDesiredLocation = GetOwner()->GetTransform().InverseTransformPosition(DesiredArmLocation);
	const FVector LocalHitLocation = GetOwner()->GetTransform().InverseTransformPosition(TraceHitLocation);
	
	// align the hit and desired local locations with the owner rotation (Yaw)
	const float DeltaRotation = OwnerCharacter->GetActorRotation().Yaw - OwnerCharacter->GetControlRotation().Yaw;
	const FVector AlignedLocalDesiredLocation = LocalDesiredLocation.RotateAngleAxis(DeltaRotation, FVector::UpVector);
	const FVector AlignedLocalHitLocation = LocalHitLocation.RotateAngleAxis(DeltaRotation, FVector::UpVector);

	// Create a new local desired location maintaining the local hit location's X and Z, and updating the Y to match the local desired location
	const FVector AlignedLocalNewDesiredLocation = FVector(AlignedLocalHitLocation.X, AlignedLocalDesiredLocation.Y, AlignedLocalHitLocation.Z);

	// Rotate the new local desired location back to the original rotation
	const FVector NewLocalDesiredLocation = AlignedLocalNewDesiredLocation.RotateAngleAxis(-DeltaRotation, FVector::UpVector);
	
	// Convert the new local desired location to world space
	const FVector NewDesiredLocation = GetOwner()->GetTransform().TransformPosition(NewLocalDesiredLocation);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	if (bShowBlendLocationDebug)
	{
		DrawDebugSphere(GetWorld(), GetOwner()->GetTransform().TransformPosition(AlignedLocalDesiredLocation), 10, 12, FColor::Blue, false, -1);
		DrawDebugSphere(GetWorld(), GetOwner()->GetTransform().TransformPosition(AlignedLocalHitLocation), 10, 12, FColor::Red, false, -1);
		DrawDebugSphere(GetWorld(), GetOwner()->GetTransform().TransformPosition(AlignedLocalNewDesiredLocation), 10, 12, FColor::Purple, false, -1);
		DrawDebugSphere(GetWorld(), NewDesiredLocation, 10, 12, FColor::Green, false, -1);
	}
#endif

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());
	FHitResult Result;
	GetWorld()->SweepSingleByChannel(Result, GetComponentLocation(), NewDesiredLocation, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);
	
	OnSpringArmHitSomething.Broadcast();
	
	return Result.bBlockingHit ? Result.Location : NewDesiredLocation;
}
