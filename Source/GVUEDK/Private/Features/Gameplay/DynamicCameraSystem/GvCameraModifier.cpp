// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/GvCameraModifier.h"

#include <string>

#include "Features/Gameplay/DynamicCameraSystem/GvSpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"

void UGvCameraModifier::Initialize()
{
}

bool UGvCameraModifier::ApplyCameraTransition(const FCameraInfo& CameraInfo, const float TransitionTime,
                                              FMinimalViewInfo& InOutPOV, const float DeltaTime)
{
	// If the target modifiers are not equal to the current ones, start a new transition
    if (!TargetModifiers.Equals(CameraInfo))
    {
        // Update the current and target modifiers
        CurrentModifiers = TargetModifiers;
        TargetModifiers = CameraInfo;

        // If there is remaining transition time, calculate the remaining time for the new transition
        RemainingTransitionTime = (RemainingTransitionTime > 0.0f) ? TotalTransitionTime - RemainingTransitionTime : TransitionTime;

        // Ensure TotalTransitionTime is set
        TotalTransitionTime = TransitionTime;
    }

    // If the current modifiers are equal to the target ones, apply the current modifiers and exit
    if (CurrentModifiers.Equals(TargetModifiers))
    {
        ApplyCameraInfo(CurrentModifiers, 1.0f, InOutPOV);
        return true;
    }
	
    // Initialize the application factor and the modifiers to apply
    float applicationFactor = 0.0f;
    FCameraInfo modifiersToApply = TargetModifiers.IsZero() ? CurrentModifiers : TargetModifiers;

    // If the remaining transition time is zero or less, start a new transition
    if (RemainingTransitionTime <= 0.0f)
    {
        RemainingTransitionTime = TotalTransitionTime = TransitionTime;
        applicationFactor = TargetModifiers.IsZero() ? 1.0f : 0.0f;
    }
    else
    {
        // Otherwise, decrement the remaining transition time
        RemainingTransitionTime -= DeltaTime;

        // Calculate the application factor based on the remaining time
        applicationFactor = TargetModifiers.IsZero() ? RemainingTransitionTime / TotalTransitionTime : 1 - (RemainingTransitionTime / TotalTransitionTime);

        // If the remaining transition time is zero or less, update the current modifiers
        if (RemainingTransitionTime <= 0.0f)
        {
            CurrentModifiers = TargetModifiers;
            applicationFactor = TargetModifiers.IsZero() ? 0.0f : 1.0f;
        }
    }

    // Apply the calculated modifiers with the application factor
    ApplyCameraInfo(modifiersToApply, applicationFactor, InOutPOV);
	return false;
}


void UGvCameraModifier::ApplyCameraInfo(const FCameraInfo& CameraInfo, const float Factor,
                                        FMinimalViewInfo& InOutPOV) const
{
    AActor* viewTarget = GetViewTarget();

	if (!IsValid(viewTarget))
	{
		return;
	}

	TArray<USceneComponent*> children;
	viewTarget->GetRootComponent()->GetChildrenComponents(true, children);
	
	// Apply FOV.
	float appliedFOV = CameraInfo.FOV * Factor;
	InOutPOV.FOV = InOutPOV.FOV + appliedFOV;

	//Apply spring arm length.
	for (int index = 0; index < children.Num(); ++index)
	{
		UGvSpringArmComponent* springArmComponent = Cast<UGvSpringArmComponent>(children[index]);
	
		if (IsValid(springArmComponent))
		{
			springArmComponent->TargetArmLengthModifier += CameraInfo.SpringArmLength * Factor;
			break;
		}
	}

	// Apply depth of field.
	if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldFocalDistance))
	{
		InOutPOV.PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
		InOutPOV.PostProcessSettings.DepthOfFieldFocalDistance += CameraInfo.DepthOfFieldFocalDistance * Factor;
	}

	if (!FMath::IsNearlyZero(CameraInfo.Position.X) || !FMath::IsNearlyZero(CameraInfo.Position.Y) || !FMath::IsNearlyZero(CameraInfo.Position.Z))
	{
		const FVector WorldToLocal = InOutPOV.Rotation.Quaternion() * CameraInfo.Position;
		InOutPOV.Location += WorldToLocal * Factor;
	}

	if (!FMath::IsNearlyZero(CameraInfo.Rotation.Pitch) || !FMath::IsNearlyZero(CameraInfo.Rotation.Yaw) || !FMath::IsNearlyZero(CameraInfo.Rotation.Roll))
	{
		InOutPOV.Rotation += CameraInfo.Rotation * Factor;
	}
}

// bool UGvCameraModifier::ApplyCameraTransition_Test(const FCameraInfo& CameraInfo, const float TransitionSpeed,
// 	FMinimalViewInfo& InOutPOV, const float DeltaTime)
// {
// 	if (!IsValid(PlayerCameraManager))
// 	{
// 		return true;
// 	}
//
// 	bool bTransitionComplete = true;
//
// 	// Apply FOV.
// 	float currentFOV = PlayerCameraManager->GetCameraCacheView().FOV;
// 	if (FMath::IsNearlyEqual(currentFOV, CameraInfo.FOV, DeltaTime * TransitionSpeed))
// 	{
// 		InOutPOV.FOV = CameraInfo.FOV;
// 	}
// 	else
// 	{
// 		currentFOV += DeltaTime * TransitionSpeed * (currentFOV > CameraInfo.FOV ? -1 : 1);
// 		InOutPOV.FOV = currentFOV;
// 		bTransitionComplete = false;
// 	}
//
// 	return bTransitionComplete;
// }

