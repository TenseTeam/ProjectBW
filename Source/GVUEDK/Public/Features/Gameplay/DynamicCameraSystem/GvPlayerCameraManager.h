// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GvPlayerCameraManager.generated.h"

class UGvCameraModifier;
/**
 * 
 */
UCLASS()
class GVUEDK_API AGvPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UGvCameraModifier*> CameraModifiers;
	
	virtual void BeginPlay() override;
	void InitializeModifiers();
};
