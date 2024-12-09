// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/DynamicCameraSystem/GvPlayerCameraManager.h"

#include "Features/Gameplay/DynamicCameraSystem/GvCameraModifier.h"

void AGvPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeModifiers();
}

void AGvPlayerCameraManager::InitializeModifiers()
{
	CameraModifiers.Empty();
	for (UCameraModifier* Element : ModifierList)
	{
		if(UGvCameraModifier* CameraModifier = Cast<UGvCameraModifier>(Element))
		{
			CameraModifier->Initialize();
			CameraModifiers.Emplace(CameraModifier);
		}
	}
}
