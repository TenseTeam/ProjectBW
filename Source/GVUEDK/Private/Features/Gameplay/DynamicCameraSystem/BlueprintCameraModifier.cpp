// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/BlueprintCameraModifier.h"

UBlueprintCameraModifier::UBlueprintCameraModifier()
{
	CustomPOVTransitionTime = 0.f;
	DefaultPov = FCameraInfo();
}

void UBlueprintCameraModifier::Initialize()
{
	if (GetWorld())
	{
		Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
		if (!Character)
		{
			bInitialized = false;
			return;
		}
	}
	
	if (!IsValid(CustomPOV))
	{
		bInitialized = false;
		return;
	}
	
	//temporary bug fix, comment out and set AimPOV.Rotation to 0, 0, 0 and any other values different from 0 to see the bug
	if (CustomPOV->POV.Rotation == FRotator::ZeroRotator)
	{
		CustomPOV->POV.Rotation.Yaw = 0.0001f;
	}

	bInitialized = true;
}

bool UBlueprintCameraModifier::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	if (!IsValid(CustomPOV))
	{
		return false;
	}
	
	Super::ModifyCamera(DeltaTime, InOutPOV);
	
	if (ApplyModifier(Character))
	{
		ApplyCameraTransition(CustomPOV->POV, CustomPOVTransitionTime, InOutPOV, DeltaTime);
		bExitTransitionCompleted = false;
		return true;
	}
	if (!bExitTransitionCompleted)
	{
		bExitTransitionCompleted = ApplyCameraTransition(DefaultPov, CustomPOVTransitionTime, InOutPOV, DeltaTime);
	}
	return false;
}


bool UBlueprintCameraModifier::ApplyModifier_Implementation(ACharacter* Context)
{
	return false;
}
