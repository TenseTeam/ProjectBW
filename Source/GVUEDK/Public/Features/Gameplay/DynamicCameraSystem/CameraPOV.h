// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraInfo.h"
#include "Engine/DataAsset.h"
#include "CameraPOV.generated.h"

/**
 * 
 */
UCLASS()
class GVUEDK_API UCameraPOV : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FCameraInfo POV;
};
