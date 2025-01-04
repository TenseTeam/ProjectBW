// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraPOV.h"
#include "GvCameraModifier.h"
#include "BlueprintCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class GVUEDK_API UBlueprintCameraModifier : public UGvCameraModifier
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ACharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraPOV* CustomPOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ToolTip = "If true, when the camera modifier is applied, no other camera modifier will be applied"))
	bool bHasPriority = false;
	FCameraInfo DefaultPov;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
	float CustomPOVTransitionTime;
	/** Used for stop the Camera Transition to be applied when the camera go back to defaults stats */
	bool bExitTransitionCompleted = true;
	

public:
	UBlueprintCameraModifier();
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	bool ApplyModifier(ACharacter* Context);
	
private:
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
	virtual void Initialize() override;
};
